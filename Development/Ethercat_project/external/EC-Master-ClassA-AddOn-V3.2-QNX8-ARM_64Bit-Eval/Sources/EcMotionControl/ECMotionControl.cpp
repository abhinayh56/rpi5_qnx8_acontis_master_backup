/*-----------------------------------------------------------------------------
 * EcMotionControl.cpp
 * Copyright                acontis technologies GmbH, Ravensburg, Germany
 * Response                 Christoph Widmann
 *                          mdu
 * Description              Motion Control functions
 *
 * No support of multiple axis function blocks
 *
 *---------------------------------------------------------------------------*/

/*- PRE INCLUDE DEFINES -----------------------------------------------------*/
#define ECMOTIONLIB_BUILD

#undef MOTIONTEST

/*-INCLUDES------------------------------------------------------------------*/
#include "EcOs.h"
#include "EcLog.h"
#include "EcEscReg.h"
#include "EthernetServices.h"
#include "ECMotionControl.h"

#include <math.h>
#include <limits.h>

/*-LOGGING-------------------------------------------------------------------*/
#if (defined INCLUDE_LOG_MESSAGES)
#define pEcLogParmsMotionDefault  (&pAxis->LogParms)
#define pEcLogParmsMotionMoveCalc (pData->pLogParms)
#define pEcLogParmsMotion         pEcLogParmsMotionDefault

#define pLogMsgCallback       (pEcLogParmsMotion)->pfLogMsg
#define pEcLogContext         (struct _EC_T_LOG_CONTEXT*)(pEcLogParmsMotion)->pLogContext
#define dwEcLogLevel          (pEcLogParmsMotion)->dwLogLevel
#endif

/*-MACROS--------------------------------------------------------------------*/

#define INPUT_PARA_CHANGED(p, lDist, fVel, fAcc, fDec)  ((p->lTotalDist != lDist) \
   || (p->fOrgMaxVelocity != fVel) || (p->fOrgAcceleration != fAcc) || (p->fOrgDeceleration != fDec))

#define _STR(s) #s
#define STR(s) _STR(s)

#define NULL_INT(a, t)           ( a * t )
#define EINFACH_INT(a, t)        (a / 2.0 * t * t)
#define ZWEIFACH_INT(a, t)       (a / 6.0 * t * t * t)

/* constant acceleration */
#define CAL_WEG_KON_B(a, t, v0)         (((v0) * t) + EINFACH_INT((a), t) )

/* decreasing acceleration */
#define CAL_WEG_AB_B(t, j, v0, a)       (((v0) * t) + EINFACH_INT((a), t) - ZWEIFACH_INT((j), t) )

/* increasing acceleration */
#define CAL_VEL_ZU_B(t, j, v0, a0)      ((v0) + ((a0) * t) + EINFACH_INT((j), t) )
#define CAL_VEL_AB_B(t, j, v0, a0)      ((v0) + ((a0) * t) - EINFACH_INT((j), t) )
#define CAL_WEG_ZU_B(t, j, v0, a )      (((v0) * t) + EINFACH_INT((a), t) + ZWEIFACH_INT((j), t ) )

/*-DEFINES-------------------------------------------------------------------*/

#define TRAJ_RAMP1              0
#define TRAJ_RAMP2              1

#define MAX_DISTANCE_SEGMENT4   1./1000.             /* 0.01 mm */

/*-GLOBAL VARIABLES-----------------------------------------------------------*/

/*-TYPEDEFS------------------------------------------------------------------*/
typedef struct
{
    EC_T_LOG_PARMS*    pLogParms;
    MC_T_REAL          fDistance;
    MC_T_REAL          fVelocity;
    MC_T_REAL          fVs;         /* start velocity */
    MC_T_REAL          fVe;         /* end velocity */
    MC_T_REAL          fMinVelocity;
    MC_T_REAL          fPeakVelNoJerk;
    MC_T_REAL          fAccReal;
    MC_T_REAL          fDecReal;
    MC_T_REAL          fJerk;
    MC_T_REAL          fTime_1;     /* time segment 1 and 3 */
    MC_T_REAL          fTime_2;
#ifdef DEBUG
    MC_T_REAL          fTime_4;
#endif
    MC_T_REAL          fTime_6;
    MC_T_REAL          fTime_7;     /* time segment 7 and 5 */
    MC_T_REAL          fVel_1;     /* velocity segment 1 and 3 */
    MC_T_REAL          fVel_2;     /* velocity segment 2 */
    MC_T_REAL          fVel_6;     /* velocity segment 6 */
    MC_T_REAL          fVel_7;     /* velocity segment 7 and 5 */
    MC_T_REAL          fDis_1;     /* distance segment 1 */
    MC_T_REAL          fDis_2;     /* distance segment 2 */
    MC_T_REAL          fDis_3;     /* distance segment 3 */
    MC_T_REAL          fDis_4;     /* distance segment 4 */
    MC_T_REAL          fDis_5;     /* distance segment 5 */
    MC_T_REAL          fDis_6;     /* distance segment 6 */
    MC_T_REAL          fDis_7;     /* distance segment 7 */
    MC_T_BOOL          bSeg1;
    MC_T_BOOL          bSeg2;
    MC_T_BOOL          bSeg6;
    MC_T_BOOL          bSeg7;
    MC_T_REAL          fCycleTime;  /* EtherCAT bus cycle time */
} MC_T_MOVE_CALC_DATA_T;


/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FORWARD DECLARATIONS------------------------------------------------------*/
static EC_T_WORD   MC_TrajSetupTrapezeMove
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_MOVEMENT*         pMove
    ,MC_T_BOOL              bNewMove
    ,MC_T_BOOL              bEndless
    ,MC_T_POS_CALC          lDistance
    ,MC_T_REAL              fMaxVelocity
    ,MC_T_REAL              fEndVelocity
    ,MC_T_REAL              fAcceleration
    ,MC_T_REAL              fDeceleration);

static EC_T_WORD   MC_TrajSetupJerkMove
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_MOVEMENT*         pMove
    ,MC_T_BOOL              bNewMove
    ,MC_T_BOOL              bEndless
    ,MC_T_POS_CALC          lDistance
    ,MC_T_REAL              fMaxVelocity
    ,MC_T_REAL              fEndVelocity
    ,MC_T_REAL              fAcceleration
    ,MC_T_REAL              fDeceleration
    ,MC_T_REAL              fJerk);

static EC_T_WORD   MC_SetupTrajInDriveMove
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_MOVEMENT*         pMove
    ,MC_T_BOOL              bNewMove
    ,MC_T_BOOL              bEndless
    ,MC_T_POS_CALC          lDistance
    ,MC_T_REAL              fMaxVelocity
    ,MC_T_REAL              fAcceleration
    ,MC_T_REAL              fDeceleration);

static EC_T_WORD   MC_TrajBreakDist
    (MC_T_MOVEMENT*         pMove);

static EC_T_WORD   MC_TrajJerkCalcAccRamp
    (MC_T_MOVEMENT*         pMove
    ,EC_T_INT               nRampIdx
    ,MC_T_VELACC            lCurrAcc
    ,MC_T_VELACC            lCurrVel
    ,MC_T_VELACC            lTargetVel
    ,MC_T_VELACC            lMaxAcc
    ,MC_T_VELACC            *plEndAcc);

static EC_T_WORD   MC_TrajJerkCalcDecRamp
    (MC_T_MOVEMENT*         pMove
    ,EC_T_INT               nRampIdx
    ,MC_T_VELACC            lCurrAcc
    ,MC_T_VELACC            lCurrVel
    ,MC_T_VELACC            lTargetVel
    ,MC_T_VELACC            lMaxDec
    ,MC_T_VELACC            *plEndAcc);

static EC_T_VOID   MC_TrajNextSetPoint
    (MC_T_MOVEMENT*         pMove);

static MC_T_VELACC   MC_TrajFollowingSetPoint
    (MC_T_MOVEMENT*         pMove);

static EC_T_WORD   MC_CheckTargetPosReached
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_BOOL*             pbDone);

static EC_T_VOID   MC_CheckTargetVelReached
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_BOOL*             pbInVelocity);

#ifdef EC_MOTION_SUPPORT_PP_MODE
static EC_T_WORD   MC_CheckHomingDone
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_BOOL*             pbDone);

static EC_T_WORD   MC_CheckTargetPosReachedTrajInDrive
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_BOOL*             pbDone);
#endif

static EC_T_WORD   MC_DriveGetInputs
    (MC_T_AXIS_REF*         pAxis);

static EC_T_WORD   MC_DriveSetTargetPosition
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_POS_CALC          lTargetPosition);

static EC_T_WORD   MC_CheckSoftwareLimits
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_POS_CALC          lDistance
    ,MC_T_POS_CALC*         pMaxDistance);

static EC_T_VOID   MC_BufferClear
    (MC_T_AXIS_REF*         pAxis);

static EC_T_VOID   MC_InitMovement
    (MC_T_MOVEMENT*         pMove
    ,MC_T_BOOL              bEndless
    ,MC_T_POS_CALC          lDistance);

static EC_T_VOID MC_SetPosition(
    MC_T_AXIS_REF*      pAxis,
    MC_T_REAL           fPosition,
    MC_T_BOOL           bRelative);

// State machine for PP mode
static EC_T_VOID MC_PPSm
    (MC_T_AXIS_REF*     pAxis);

/*-FUNCTIONS-----------------------------------------------------------------*/
// band matrix solver
class band_matrix
{
private:
    MC_T_REAL     m_upper[2][MAX_NUM_OF_SPLINE_POINTS];     // upper band
    MC_T_REAL     m_lower[2][MAX_NUM_OF_SPLINE_POINTS];     // lower band

    EC_T_INT      m_upper_size;                             // matrix dimension
    EC_T_INT      m_lower_size;
    EC_T_INT      m_dim;

public:
    band_matrix() {};                               // constructor
    band_matrix(MC_T_INT dim);                      // constructor

    ~band_matrix() {};                              // destructor
    EC_T_DWORD resize(MC_T_INT dim);                // init with dim

    MC_T_INT num_upper() const
    {
        return m_upper_size-1;
    }

    MC_T_INT num_lower() const
    {
        return m_lower_size-1;
    }

    // access operator
    MC_T_REAL & operator () (MC_T_INT i, MC_T_INT j);            // write
    MC_T_REAL   operator () (MC_T_INT i, MC_T_INT j) const;      // read

    // we can store an additional diogonal (in m_lower)
    MC_T_REAL& saved_diag(MC_T_INT i);
    MC_T_REAL  saved_diag(MC_T_INT i) const;
    void lu_decompose();

    EC_T_VOID r_solve(MC_T_REAL* pB, MC_T_REAL* pX);
    EC_T_VOID l_solve(MC_T_REAL* pB, MC_T_REAL* pX);
    EC_T_VOID lu_solve(MC_T_REAL* pB, MC_T_REAL* pX, bool is_lu_decomposed=false);
};

static inline MC_T_BOOL IsExclusive(MC_T_BOOL t1, MC_T_BOOL t2, MC_T_BOOL t3 = MC_FALSE, MC_T_BOOL t4 = MC_FALSE)
{
   return t1 + t2 + t3 + t4 <= 1;
}

static inline MC_T_BOOL IsAnyOn(MC_T_BOOL t1, MC_T_BOOL t2, MC_T_BOOL t3 = MC_FALSE, MC_T_BOOL t4 = MC_FALSE)
{
   return t1 + t2 + t3 + t4 >= 1;
}

static inline const char * const FbState2Str(MC_T_FBSTATE fbState)
{
   switch (fbState)
   {
   case FBS_IDLE: return STR(FBS_IDLE);
   case FBS_BUSY: return STR(FBS_BUSY);
   case FBS_ACTIVE: return STR(FBS_ACTIVE);
   case FBS_DONE: return STR(FBS_DONE);
   case FBS_ERROR: return STR(FBS_ERROR);
   case FBS_COMMAND_ABORTED: return STR(FBS_COMMAND_ABORTED);
   default: return "UNKNOWN";
   }
}

static inline const char * const SercosState2Str(MC_T_SERCOS_STATE sercosState)
{
   switch (sercosState)
   {
   case DRV_SER_STATE_NOT_READY: return STR(DRV_SER_STATE_NOT_READY);
   case DRV_SER_STATE_READY_TO_SWITCHON: return STR(DRV_SER_STATE_READY_TO_SWITCHON);
   case DRV_SER_STATE_SWITCHED_ON: return STR(DRV_SER_STATE_SWITCHED_ON);
   case DRV_SER_STATE_OP_ENABLED: return STR(DRV_SER_STATE_OP_ENABLED);
   case DRV_SER_STATE_MALFUNCTION: return STR(DRV_SER_STATE_MALFUNCTION);
   default: return "UNKNOWN";
   }
}

const char * const MC_FB_T::GetName() const
{
   switch (this->wFbID)
   {
   case MCFB_ID_POWER: return "MC_POWER_T";
   case MCFB_ID_HOME: return "MC_HOME_T";
   case MCFB_ID_STOP: return "MC_STOP_T";
   case MCFB_ID_HALT: return "MC_HALT_T";
   case MCFB_ID_MOVE_ABSOLUTE: return "MC_MOVE_ABSOLUTE_T";
   case MCFB_ID_MOVE_RELATIVE: return "MC_MOVE_RELATIVE_T";
   case MCFB_ID_MOVE_VELOCITY: return "MC_MOVE_VELOCITY_T";
   case MCFB_ID_MOVE_CONTINUOUS_ABSOLUTE: return "MC_MOVE_CONT_ABSOLUTE_T";
   case MCFB_ID_MOVE_CONTINUOUS_RELATIVE: return "MC_MOVE_CONT_RELATIVE_T";
   case MCFB_ID_POSITION_PROFILE: return "MC_POSITION_PROFILE_T";
   case MCFB_ID_SET_POSITION: return "MC_SET_POSITION_T";
   case MCFB_ID_READ_PARAMETER: return "MC_READ_PARAMETER_T";
   case MCFB_ID_READ_BOOL_PARAMETER: return "MC_READ_BOOL_PARAMETER_T";
   case MCFB_ID_WRITE_PARAMETER: return "MC_WRITE_PARAMETER_T";
   case MCFB_ID_WRITE_BOOL_PARAMETER: return "MC_WRITE_BOOL_PARAMETER_T";
   case MCFB_ID_READ_DIGITAL_INPUT: return "MC_READ_DIGITAL_INPUT_T";
   case MCFB_ID_READ_DIGITAL_OUTPUT: return "MC_READ_DIGITAL_OUTPUT_T";
   case MCFB_ID_WRITE_DIGITAL_OUTPUT: return "MC_WRITE_DIGITAL_OUTPUT_T";
   case MCFB_ID_READ_ACTUAL_POSITION: return "MC_READ_ACTUAL_POSITION_T";
   case MCFB_ID_READ_ACTUAL_VELOCITY: return "MC_READ_ACTUAL_VELOCITY_T";
   case MCFB_ID_READ_MOTION_STATE: return "MC_READ_MOTION_STATE_T";
   case MCFB_ID_READ_AXIS_INFO: return "MC_READ_AXIS_INFO_T";
   case MCFB_ID_READ_AXIS_ERROR: return "MC_READ_ERROR_T";
   case MCFB_ID_RESET: return "MC_RESET_T";
   case MCFB_ID_CAMTABLE_SELECT: return "MC_CAMTABLE_SELECT_T";
   case MCFB_ID_CAM_IN: return "MC_CAM_IN_T";
   case MCFB_ID_CAM_OUT: return "MC_CAM_OUT_T";
   case MCFB_ID_GEAR_IN: return "MC_GEAR_IN_T";
   case MCFB_ID_GEAR_OUT: return "MC_GEAR_OUT_T";
   case MCFB_ID_CHECK_TARGETPOS_REACHED: return "AMC_CHECK_TARGETPOS_REACHED_T";
   case MCFB_ID_HALT_RECOVERY: return "AMC_HALT_RECOVERY_T";
   default: return "MC_Unknown";
   }
}

void MC_FB_T::ChangeState(MC_T_FBSTATE newState)
{
   if (pAxis != EC_NULL)
   {
      EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "Axis=%4d: %s State: Cycle=%d   Prev=%s -> Curr=%s\n",
          pAxis->wStationAddress, GetName(),  pAxis->dwCycleCount, FbState2Str(this->fbState), FbState2Str(newState)));
   }

   this->fbState = newState;
   if(pAxis != EC_NULL) this->dwStateChCycle = pAxis->dwCycleCount;
}


MC_T_AXIS_REF::MC_T_AXIS_REF()
{
    OsMemset(this, 0, sizeof(*this));
}

MC_T_AXIS_REF::MC_T_AXIS_REF(const MC_T_AXIS_INIT &init)
{
    OsMemset(this, 0, sizeof(*this));
    Init(init);
}

void MC_T_AXIS_REF::Init(const MC_T_AXIS_INIT &init)
{
    eAxisType = init.AxisType;
    dwCycleTime = (MC_T_DWORD) init.CycleTime;
    dwIncPerMM = (MC_T_DWORD) init.IncPerMM;
    lIncFactor = (MC_T_DWORD) (1<<init.IncFactor);
    LogParms = init.LogParms;
    dwVelocityGain = (MC_T_DWORD) init.VelocityGain;
    dwTorqueGain   = (MC_T_DWORD) init.TorqueGain;
}

void MC_T_AXIS_REF::InitInputs(const MC_T_AXIS_INIT_INPUTS &init)
{
   this->plPdActualPosition = init.pActualPosition;
   this->psPdActualTorque = init.pActualTorque;
   this->pdwDigitalInputs = init.pDigitalInputs;
}

void MC_T_AXIS_REF::InitOutputs(const MC_T_AXIS_INIT_OUTPUTS &init)
{
   this->plPdTargetPosition = init.pTargetPosition;
   this->plPdTargetVelocity = init.pTargetVelocity;
   this->psPdTargetTorque = init.pTargetTorque;
   this->plPdVelocityOffset = init.pVelocityOffset;
   this->psPdTorqueOffset = init.pTorqueOffset;
   this->pbyPdModeOfOperation = init.pModeOfOperation;
   this->pbyPdModeOfOperationDisplay = init.pModeOfOperationDisplay;
   this->pdwDigitalOutputs = init.pDigitalOutputs;
#ifdef EC_MOTION_SUPPORT_PP_MODE
   this->pdwPdProfileVelocity = init.pProfileVelocity;
   this->pdwPdProfileAcc = init.pProfileAcc;
   this->pdwPdProfileDec = init.pProfileDec;
#endif
}

/***************************************************************************************************/
/**
\brief  Initialize axis structure (for EtherCAT operation)

If used, this function shall be called at least once before processing the MCFB's.
If not called, the motion API will work, but it remains in simulation mode.

*/
void MC_T_AXIS_REF::InitEcat(const MC_T_AXIS_INIT_ECAT &init)
{
   this->dwMasterInstanceID = init.dwMasterInstanceID;
   this->dwSlaveID = init.SlaveID;
   this->wStationAddress = init.StationAddress;

   this->pEcatCoeSdoDownload = init.pEcatCoeSdoDownload;
   this->pEcatCoeSdoUpload = init.pEcatCoeSdoUpload;
   this->pEcatSoeWrite = init.pEcatSoeWrite;
   this->pEcatSoeRead = init.pEcatSoeRead;
   this->pEcatGetSlaveState = init.pEcatGetSlaveState;

   this->eProfile = (init.Profile == MC_T_AXIS_PROFILE_NONE) ? MC_T_AXIS_PROFILE_DS402 : init.Profile;
   this->bySercosDriveNo = init.SercosDriveNo;
   this->wCoeIdxOpMode = init.CoeIdxOpMode;
   this->wCoeSubIdxOpMode = init.CoeSubIdxOpMode;
   this->pwPdStatusWord = init.pStatusWord;
   this->pwPdControlWord = init.pControlWord;
}

/***************************************************************************************************/
/**
 \brief     Reads drive controller error code (object 0x603F)
            and stores it into buffer pointed by pwErrorCode

 \detail    May not be called within EC-Master cyclic task,
            due to blocking call to SDO upload function

 \param     MC_T_WORD* pwErrorCode     pointer to buffer

 \return    MC Error code if fails, EC_E_NOERROR otherwise
*/
MC_T_DWORD MC_T_AXIS_REF::GetErrorCode(MC_T_WORD* pwErrorCode)
{
    MC_T_WORD   wErrorCode = 0;
    EC_T_DWORD  dwRes = EC_E_NOERROR;
    EC_T_DWORD  dwOutDataLen = 0;

    if (pwErrorCode == EC_NULL)
        return EC_E_INVALIDPARM;

    dwRes = this->pEcatCoeSdoUpload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_ERROR_CODE, 0,
        (EC_T_BYTE *)&wErrorCode, sizeof(wErrorCode), &dwOutDataLen, EC_SDO_UP_DOWN_TIMEOUT, 0);

    if ((dwRes == EC_E_NOERROR) && (dwOutDataLen == sizeof(wErrorCode)))
    {
        *pwErrorCode = wErrorCode;
    }

    return dwRes;
}

/***************************************************************************************************/
/**
\brief  Set Mode of Operation

* \return N/A
*/
EC_T_DWORD MC_T_AXIS_REF::SetModeOfOperation(EC_T_WORD wProfileOperationModeParam)
{
   EC_T_DWORD      dwRes = MC_NO_ERROR;

   if (!IsOpModeValid(wProfileOperationModeParam)) return MC_ERR_INVALID_OPERATION_MODE;

   /* save operation mode */
   this->wProfileOperationMode = wProfileOperationModeParam;
   this->eOperationMode = MapOpMode();

   if (this->eAxisType == MC_AXIS_TYPE_REAL_ALL)
   {
      switch (this->eProfile)
      {
      case MC_T_AXIS_PROFILE_DS402:
         {
            EC_T_BYTE byOpMode = (EC_T_BYTE) this->wProfileOperationMode;

            if(this->pbyPdModeOfOperation != EC_NULL)
            {
               *this->pbyPdModeOfOperation = byOpMode;
            }
            else
            {
                dwRes = this->pEcatCoeSdoDownload
                   ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, this->wCoeIdxOpMode, (EC_T_BYTE)this->wCoeSubIdxOpMode,
                       (EC_T_BYTE *)&byOpMode, sizeof(byOpMode), EC_SDO_UP_DOWN_TIMEOUT, 0)
                   : EC_E_NOTSUPPORTED;

                /* workaround for drives which expects a 16 bit value here */
                if (dwRes == EC_E_SDO_ABORTCODE_DATA_LENGTH_NOT_MATCH)
                {
                   dwRes = this->pEcatCoeSdoDownload
                      ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, this->wCoeIdxOpMode, (EC_T_BYTE)this->wCoeSubIdxOpMode,
                         (EC_T_BYTE *)&this->wProfileOperationMode, sizeof(EC_T_WORD), EC_SDO_UP_DOWN_TIMEOUT, 0)
                      : EC_E_NOTSUPPORTED;
                }
            }
            break;
         };
      case MC_T_AXIS_PROFILE_SERCOS:
         {
            EC_T_BYTE flags = SOE_BM_ELEMENTFLAG_VALUE;
            dwRes = this->pEcatSoeWrite
               ? this->pEcatSoeWrite(this->dwMasterInstanceID, this->dwSlaveID, this->bySercosDriveNo, &flags, DRV_IDN_OPMODE,
                  (EC_T_BYTE *)&this->wProfileOperationMode, sizeof(EC_T_WORD), EC_SDO_UP_DOWN_TIMEOUT)
               : EC_E_NOTSUPPORTED;
            break;
         };

      default:
      case MC_T_AXIS_PROFILE_NONE:
         OsDbgAssert(EC_FALSE);
         break;
      }
   }

   return dwRes;
}

MC_T_DWORD MC_T_AXIS_REF::SetHomingParameters(
    MC_T_BYTE byMethod,             /* Homing method */
    MC_T_REAL fSpeedSearchSwitch,   /* Speed during search for switch [mm/s] */
    MC_T_REAL fSpeedSearchZero,     /* Speed during search for zero [mm/s] */
    MC_T_REAL fAcceleration,        /* Homing acceleration [mm/s^2] */
    MC_T_REAL fOffset               /* Home offset [mm] */
    )
{
    EC_T_DWORD      dwRes = MC_NO_ERROR;
    EC_T_DWORD      dwValue = 0;

    if (this->eAxisType == MC_AXIS_TYPE_REAL_ALL)
    {
        switch (this->eProfile)
        {
        case MC_T_AXIS_PROFILE_DS402:
        {
            dwRes = this->pEcatCoeSdoDownload
                ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_HOMING_METHOD, 0,
                (EC_T_BYTE *)&byMethod, sizeof(byMethod), EC_SDO_UP_DOWN_TIMEOUT, 0)
                : EC_E_NOTSUPPORTED;
            if (dwRes != EC_E_NOERROR)
            {
                return dwRes;
            }

            dwValue = (EC_T_DWORD)(fSpeedSearchSwitch * this->fVelToInc);
            dwRes = this->pEcatCoeSdoDownload
                ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_HOMING_SPEED, DRV_IDN_HOMING_SEARCH_SPEED_SWITCH,
                (EC_T_BYTE *)&dwValue, sizeof(dwValue), EC_SDO_UP_DOWN_TIMEOUT, 0)
                : EC_E_NOTSUPPORTED;
            if (dwRes != EC_E_NOERROR)
            {
                return dwRes;
            }

            dwValue = (EC_T_DWORD)(fSpeedSearchZero * this->fVelToInc);
            dwRes = this->pEcatCoeSdoDownload
                ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_HOMING_SPEED, DRV_IDN_HOMING_SEARCH_SPEED_ZERO,
                (EC_T_BYTE *)&dwValue, sizeof(dwValue), EC_SDO_UP_DOWN_TIMEOUT, 0)
                : EC_E_NOTSUPPORTED;
            if (dwRes != EC_E_NOERROR)
            {
                return dwRes;
            }

            dwValue = (EC_T_DWORD)(fAcceleration * this->fAccToInc);
            dwRes = this->pEcatCoeSdoDownload
                ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_HOMING_ACCELERATION, 0,
                (EC_T_BYTE *)&dwValue, sizeof(dwValue), EC_SDO_UP_DOWN_TIMEOUT, 0)
                : EC_E_NOTSUPPORTED;
            if (dwRes != EC_E_NOERROR)
            {
                return dwRes;
            }

            dwValue = (EC_T_DWORD)(fOffset * this->dwIncPerMM);
            dwRes = this->pEcatCoeSdoDownload
                ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_HOMING_OFFSET, 0,
                (EC_T_BYTE *)&dwValue, sizeof(dwValue), EC_SDO_UP_DOWN_TIMEOUT, 0)
                : EC_E_NOTSUPPORTED;
            if (dwRes != EC_E_NOERROR)
            {
                return dwRes;
            }

            break;
        };
        case MC_T_AXIS_PROFILE_SERCOS:
        default:
            dwRes = EC_E_NOTSUPPORTED;
            break;
        }
    }

    return dwRes;
}

/***************************************************************************************************/
/**
\brief  Check if software limit objects are supported

* \return N/A
*/
EC_T_DWORD MC_T_AXIS_REF::SetSoftwareLimits
   (MC_T_BOOL     bEnaLimitPos
   ,MC_T_BOOL     bEnaLimitNeg
   ,MC_T_REAL     fLimitPos
   ,MC_T_REAL     fLimitNeg)

{
   EC_T_DWORD      dwRes = MC_NO_ERROR;
   EC_T_BYTE       bySubindex0 = 0;
   EC_T_DWORD      dwOutDataLen = 0;
   MC_T_BOOL       bSWLimitObjSupported = MC_FALSE;
   MC_T_SDWORD     sdwLimit = 0;

   if (this->eAxisType == MC_AXIS_TYPE_REAL_ALL)
   {
      switch (this->eProfile)
      {
      case MC_T_AXIS_PROFILE_DS402:
         {
            if((this->pEcatCoeSdoUpload != EC_NULL) && (this->pEcatCoeSdoDownload != EC_NULL))
            {
               dwRes = this->pEcatCoeSdoUpload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_SOFTWARE_POSITION_LIMIT, 0,
                        (EC_T_BYTE *)&bySubindex0, sizeof(bySubindex0), &dwOutDataLen, EC_SDO_UP_DOWN_TIMEOUT, 0);

               if(dwRes == EC_E_NOERROR)
               {
                  /* check if object is available */
                  bSWLimitObjSupported = (bySubindex0 == 2) ?  EC_TRUE : EC_FALSE;

                  /* set positive limit: SubIndex 2 */
                  if(bSWLimitObjSupported)
                  {
                     this->bSWLimitPositiveEna = bEnaLimitPos;
                     this->lSWLimitPositivePos = (MC_T_POS_CALC)(fLimitPos *  (MC_T_POS_CALC)this->dwCalcIncPerMM);

                     /* if not enabled, set value to 0 */
                     sdwLimit = bEnaLimitPos ? (MC_T_SDWORD)(this->lSWLimitPositivePos/this->lIncFactor) : 0;

                     dwRes = this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_SOFTWARE_POSITION_LIMIT, 2,
                              (EC_T_BYTE *)&sdwLimit, sizeof(sdwLimit), EC_SDO_UP_DOWN_TIMEOUT, 0);
                  }

                  /* set negative limit: SubIndex 1 */
                  if(bSWLimitObjSupported)
                  {
                     this->bSWLimitNegativeEna = bEnaLimitNeg;
                     this->lSWLimitNegativePos = (MC_T_POS_CALC)(fLimitNeg *  (MC_T_POS_CALC)this->dwCalcIncPerMM);

                     /* if not enabled, set value to 0 */
                     sdwLimit = bEnaLimitNeg ? (MC_T_SDWORD)(this->lSWLimitNegativePos/this->lIncFactor) : 0;

                     dwRes = this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_SOFTWARE_POSITION_LIMIT, 1,
                              (EC_T_BYTE *)&sdwLimit, sizeof(sdwLimit), EC_SDO_UP_DOWN_TIMEOUT, 0);
                  }
               }
            }
            else
            {
               dwRes = EC_E_NOTSUPPORTED;
            }
            break;
         };
      case MC_T_AXIS_PROFILE_SERCOS:
      default:
            dwRes = EC_E_NOTSUPPORTED;
            break;
      }
   }

   return dwRes;
}

/***************************************************************************************************/
/**
\brief  Check if software limit objects are supported

* \return N/A
*/
EC_T_DWORD MC_T_AXIS_REF::SetPositionRangeLimits
    (MC_T_BOOL      bEnaLimitPos
    , MC_T_BOOL     bEnaLimitNeg
    , MC_T_REAL     fLimitPos
    , MC_T_REAL     fLimitNeg
    )
{
    EC_T_DWORD      dwRes = MC_NO_ERROR;
    EC_T_BYTE       bySubindex0 = 0;
    MC_T_BOOL       bSWLimitObjSupported = MC_FALSE;
    EC_T_DWORD      dwOutDataLen = 0;
    MC_T_SDWORD     sdwLimit = 0;

    if (this->eAxisType == MC_AXIS_TYPE_REAL_ALL)
    {
        switch (this->eProfile)
        {
        case MC_T_AXIS_PROFILE_DS402:
        {
            if ((this->pEcatCoeSdoUpload != EC_NULL) && (this->pEcatCoeSdoDownload != EC_NULL))
            {
                dwRes = this->pEcatCoeSdoUpload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_POSITION_RANGE_LIMIT, 0,
                    (EC_T_BYTE *)&bySubindex0, sizeof(bySubindex0), &dwOutDataLen, EC_SDO_UP_DOWN_TIMEOUT, 0);

                if (dwRes == EC_E_NOERROR)
                {
                    /* check if object is available */
                    bSWLimitObjSupported = (bySubindex0 == 2) ? EC_TRUE : EC_FALSE;

                    /* set positive limit: SubIndex 2 */
                    if (bSWLimitObjSupported)
                    {
                        this->bSWLimitPositiveEna = bEnaLimitPos;
                        this->lSWLimitPositivePos = (MC_T_POS_CALC)(fLimitPos *  (MC_T_POS_CALC)this->dwCalcIncPerMM);

                        /* if not enabled, set value to 0 */
                        sdwLimit = bEnaLimitPos ? (MC_T_SDWORD)(this->lSWLimitPositivePos / this->lIncFactor) : 0;

                        dwRes = this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_POSITION_RANGE_LIMIT,
                            DRV_IDN_POSITION_RANGE_LIMIT_MAX,
                            (EC_T_BYTE *)&sdwLimit, sizeof(sdwLimit), EC_SDO_UP_DOWN_TIMEOUT, 0);
                    }

                    /* set negative limit: SubIndex 1 */
                    if (bSWLimitObjSupported)
                    {
                        this->bSWLimitNegativeEna = bEnaLimitNeg;
                        this->lSWLimitNegativePos = (MC_T_POS_CALC)(fLimitNeg *  (MC_T_POS_CALC)this->dwCalcIncPerMM);

                        /* if not enabled, set value to 0 */
                        sdwLimit = bEnaLimitNeg ? (MC_T_SDWORD)(this->lSWLimitNegativePos / this->lIncFactor) : 0;

                        dwRes = this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_POSITION_RANGE_LIMIT,
                            DRV_IDN_POSITION_RANGE_LIMIT_MIN,
                            (EC_T_BYTE *)&sdwLimit, sizeof(sdwLimit), EC_SDO_UP_DOWN_TIMEOUT, 0);
                    }
                }
            }
            else
            {
                dwRes = EC_E_NOTSUPPORTED;
            }
            break;
        }
        case MC_T_AXIS_PROFILE_SERCOS:
        default:
            dwRes = EC_E_NOTSUPPORTED;
            break;
        }
    }

    return dwRes;
}



/***************************************************************************************************/
/**
\brief  Set Motion Profile Type

* \return N/A
*/
EC_T_DWORD MC_T_AXIS_REF::SetMotionProfileType(EC_T_SWORD sProfileOperationMode)
{
   EC_T_DWORD      dwRes = MC_NO_ERROR;

   if (this->eAxisType == MC_AXIS_TYPE_REAL_ALL)
   {
      switch (this->eProfile)
      {
      case MC_T_AXIS_PROFILE_DS402:
         {
            dwRes = this->pEcatCoeSdoDownload
               ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_MOTION_PROFILE_TYPE, 0,
                   (EC_T_BYTE *)&sProfileOperationMode, sizeof(sProfileOperationMode), EC_SDO_UP_DOWN_TIMEOUT, 0)
               : EC_E_NOTSUPPORTED;
            break;
         };
      case MC_T_AXIS_PROFILE_SERCOS:
      default:
            dwRes = EC_E_NOTSUPPORTED;
            break;
      }
   }

   return dwRes;
}


/***************************************************************************************************/
/**
\brief  Set Position Option Code

* \return N/A
*/
EC_T_DWORD MC_T_AXIS_REF::SetProfileOptionCode(EC_T_WORD wPosOptionCode)
{
   EC_T_DWORD      dwRes = MC_NO_ERROR;

   if (this->eAxisType == MC_AXIS_TYPE_REAL_ALL)
   {
      switch (this->eProfile)
      {
      case MC_T_AXIS_PROFILE_DS402:
         {
            dwRes = this->pEcatCoeSdoDownload
               ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_POS_OPTION_MODE, 0,
                   (EC_T_BYTE *)&wPosOptionCode, sizeof(wPosOptionCode), EC_SDO_UP_DOWN_TIMEOUT, 0)
               : EC_E_NOTSUPPORTED;
            break;
         };
      case MC_T_AXIS_PROFILE_SERCOS:
      default:
            dwRes = EC_E_NOTSUPPORTED;
            break;
      }
   }

   return dwRes;
}


/***************************************************************************************************/
/**
\brief  Set Position Window

* \return N/A
*/
EC_T_DWORD MC_T_AXIS_REF::SetPositionWindow(MC_T_DWORD dwWindow, MC_T_WORD wTime)
{
   EC_T_DWORD      dwRes = MC_NO_ERROR;

   if (this->eAxisType == MC_AXIS_TYPE_REAL_ALL)
   {
      switch (this->eProfile)
      {
      case MC_T_AXIS_PROFILE_DS402:
         {
            dwRes = this->pEcatCoeSdoDownload
               ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_POSITION_WINDOW, 0,
                   (EC_T_BYTE *)&dwWindow, sizeof(dwWindow), EC_SDO_UP_DOWN_TIMEOUT, 0)
               : EC_E_NOTSUPPORTED;

            if(dwRes == MC_NO_ERROR)
            {
               dwRes = this->pEcatCoeSdoDownload
                  ? this->pEcatCoeSdoDownload(this->dwMasterInstanceID, this->dwSlaveID, DRV_OBJ_POSITION_WINDOW_TIME, 0,
                      (EC_T_BYTE *)&wTime, sizeof(wTime), EC_SDO_UP_DOWN_TIMEOUT, 0)
                  : EC_E_NOTSUPPORTED;
            }
            break;
         };
      case MC_T_AXIS_PROFILE_SERCOS:
      default:
            dwRes = EC_E_NOTSUPPORTED;
            break;
      }
   }

   return dwRes;
}



/********************************************************************************************/
/**
\brief  Get log message

\return -
*/
EC_T_DWORD MC_T_AXIS_REF::GetLogMsg(EC_T_CHAR** pszLog)
{
    MC_T_VELACC                 lTargetPosStep;
    if((labs(this->lActualVelocity) > 5) || (this->oMove.lLastTargetPosStep != 0))
    {
        lTargetPosStep = (MC_T_VELACC)this->oMove.lLastTargetPosStep;
        if(this->dwDirection == MC_DIR_NEGATIVE)    lTargetPosStep = -lTargetPosStep;

        /* time[ms];CycCnt; Act.Pos [Inc];Act. Vel [Inc];Comm.Pos [Inc]; Cont Err; Traj.State; Step [Inc]; */
        OsSnprintf(this->szLog, MC_LOG_BUFFER_SIZE, "%d;%d;%d;%d;%d;%d;%d;%d;", OsQueryMsecCount(), this->dwCycleCount,
            (MC_T_POSITION)(this->lActualPosition/this->lIncFactor),
            this->lActualVelocity, (MC_T_POSITION)(this->lCommandedPosition/this->lIncFactor),
            (MC_T_POSITION)((this->lCommandedPosition-this->lActualPosition)/this->lIncFactor), this->oMove.eTrajectState, lTargetPosStep/this->lIncFactor);

        *pszLog = &this->szLog[0];
    }
    else
    {
        *pszLog = EC_NULL;
    }

    return EC_E_NOERROR;
}

MC_T_BOOL MC_T_AXIS_REF::IsOpModeValid(EC_T_WORD wOpMode)
{
   if (this->eAxisType == MC_AXIS_TYPE_REAL_ALL)
   {
      switch (this->eProfile)
      {
      case MC_T_AXIS_PROFILE_DS402:
         {
            return wOpMode == DRV_MODE_OP_CSP
               || wOpMode == DRV_MODE_OP_CSV
               || wOpMode == DRV_MODE_OP_CST
               || wOpMode == DRV_MODE_OP_INTER_POS
               || wOpMode == DRV_MODE_OP_PROF_POS
               || wOpMode == DRV_MODE_OP_PROF_VEL;
         };
      case MC_T_AXIS_PROFILE_SERCOS:
         {
            return wOpMode == SER_OPMODE_VEL
               || wOpMode == SER_OPMODE_POS_FB1
               || wOpMode == SER_OPMODE_POS_FB2
               || wOpMode == SER_OPMODE_POS_FB1FB2
               || wOpMode == SER_OPMODE_POS_FB1_LAGLESS
               || wOpMode == SER_OPMODE_POS_FB2_LAGLESS
               || wOpMode == SER_OPMODE_POS_FB1FB2_LAGLESS;
         };

      default:
      case MC_T_AXIS_PROFILE_NONE:
         OsDbgAssert(EC_FALSE);
         break;
      }
   }
   else
   {
      return wOpMode == OPMODE_CSV || wOpMode == OPMODE_CSP || wOpMode == OPMODE_CST;
   }

   return false;
}

/* Check if Bit 12: Drive follows the command value is supported */
MC_T_BOOL MC_T_AXIS_REF::CheckDrvFollowBitSupported(EC_T_WORD wOpMode)
{
    MC_T_BOOL bDriveFollowBitSuppLocal = MC_FALSE;

   if (this->eAxisType == MC_AXIS_TYPE_REAL_ALL)
   {
      switch (this->eProfile)
      {
      case MC_T_AXIS_PROFILE_DS402:
         if((wOpMode == DRV_MODE_OP_CSP) || (wOpMode == DRV_MODE_OP_CSV) || (wOpMode == DRV_MODE_OP_CST))
         {
             bDriveFollowBitSuppLocal = MC_TRUE;
         }
         break;

      case MC_T_AXIS_PROFILE_SERCOS:
          break;

      default:
      case MC_T_AXIS_PROFILE_NONE:
         OsDbgAssert(EC_FALSE);
         break;
      }
   }
   else
   {
       bDriveFollowBitSuppLocal = MC_TRUE;
   }

   return bDriveFollowBitSuppLocal;
}

MC_T_OPMODE MC_T_AXIS_REF::MapOpMode()
{
   EC_T_WORD wOpMode = this->wProfileOperationMode;

   if (this->eAxisType != MC_AXIS_TYPE_REAL_ALL) return (MC_T_OPMODE) wOpMode;

   switch (this->eProfile)
   {
   case MC_T_AXIS_PROFILE_DS402:
   {
       eSetValSourceOrg = eSetValSource = SETVAL_TRAJGEN_ECM;       /* default trajectory created by EC-motion lib */
       switch (wOpMode)
       {
       case DRV_MODE_OP_PROF_POS:
       case DRV_MODE_OP_PROF_VEL:
           eSetValSourceOrg = eSetValSource = SETVAL_TRAJGEN_DRV;   /* trajectory created by drive */
           return OPMODE_PP;
       case DRV_MODE_OP_HOMING:
           eSetValSourceOrg = eSetValSource = SETVAL_TRAJGEN_DRV;  /* trajectory created by drive */
           return OPMODE_HOMING;
       
       case DRV_MODE_OP_CSP:
       case DRV_MODE_OP_INTER_POS:
           return OPMODE_CSP;

       case DRV_MODE_OP_CSV:
           return OPMODE_CSV;

       case DRV_MODE_OP_CST:
           return OPMODE_CST;
       }
   };
   case MC_T_AXIS_PROFILE_SERCOS:
   {
       switch (wOpMode)
       {
       case SER_OPMODE_POS_FB1:
       case SER_OPMODE_POS_FB2:
       case SER_OPMODE_POS_FB1FB2:
       case SER_OPMODE_POS_FB1_LAGLESS:
       case SER_OPMODE_POS_FB2_LAGLESS:
       case SER_OPMODE_POS_FB1FB2_LAGLESS:
           return OPMODE_CSP;
       case SER_OPMODE_VEL:
           return OPMODE_CSV;
       }
   };
   default:
   case MC_T_AXIS_PROFILE_NONE:
       OsDbgAssert(EC_FALSE);
       break;
   }

   return OPMODE_NOTSET;
}

#ifdef EC_MOTION_SUPPORT_MC_CAMMING
MC_T_WORD MC_T_CAM_ID::CalcSlavePos(MC_T_REAL fMasterPosPhysical, MC_T_REAL* pfTargetPos, MC_T_BOOL* bValidPos)
{
    MC_T_WORD    wErrorId = MC_NO_ERROR;
    MC_T_REAL    fSlavePos;
    MC_T_REAL    fMasterPos;
    MC_T_BOOL    bProfileShifted;
    MC_T_INT     nNumOfShifts = 0;
    MC_T_BOOL    bValidMasterPos = MC_TRUE;

    /* don't clear this if we aren't in periodic mode */
    if(this->Periodic)
        bEndOfProfile = MC_FALSE;


    if(bEndOfProfile)   /* don't move slave axis anymore if we saw bEndOfProfile in non-periodic mode */
    {
        *bValidPos = MC_FALSE;
        return MC_NO_ERROR;
    }

    do
    {
        bProfileShifted = MC_FALSE;
        fMasterPos = ((fMasterPosPhysical - this->fMasterStartOffset) * fMasterScaling) + this->fMasterOffset;

        /* is the axis out of profile ? */
        if(this->MasterAxis->dwDirection == MC_DIR_POSITIVE)
        {
            /* is the axis leaving the current range ? */
            if(fMasterPos > this->fMasterEnd)
            {
                bEndOfProfile = MC_TRUE;
                if(this->Periodic)
                {
                    /* shift cam table */
                    this->fMasterStartOffset = this->fMasterStartOffset + this->fMasterPeriod;
                    bProfileShifted = MC_TRUE; nNumOfShifts++;
                }
                else
                {
                    bValidMasterPos = MC_FALSE;     /* outside of cam table */
                }
            }
            else if(fMasterPos < this->fMasterStart)
            {
                bValidMasterPos = MC_FALSE;         /* outside of cam table */
            }
        }
        else if(this->MasterAxis->dwDirection == MC_DIR_NEGATIVE)
        {
            if(fMasterPos < this->fMasterStart)
            {
                bEndOfProfile = MC_TRUE;
                if(this->Periodic)
                {
                    /* shift cam table */
                    this->fMasterStartOffset = this->fMasterStartOffset - this->fMasterPeriod;
                    bProfileShifted = MC_TRUE; nNumOfShifts++;
                }
                else
                {
                    bValidMasterPos = MC_FALSE;     /* outside of cam table */
                }
            }
            else if(fMasterPos > this->fMasterEnd)
            {
                bValidMasterPos = MC_FALSE;         /* outside of cam table */
            }
        }
    }
    while(bProfileShifted && (nNumOfShifts < 10));

    if(nNumOfShifts == 10)
    {
        wErrorId = MC_ERR_CAM_IN_MASPOS_TOO_BIG;
        bValidMasterPos = MC_FALSE;
    }

    if(bValidMasterPos && (wErrorId == MC_NO_ERROR))
    {
        /* calculate slave pos based on master pos */
        fSlavePos = this->pSpline->get_result(fMasterPos);

        fSlavePos = (fSlavePos*fSlaveScaling) + this->fSlaveOffset + this->fSlaveStartOffset;

        *pfTargetPos = fSlavePos;
        *bValidPos = MC_TRUE;
    }
    else
    {
        *bValidPos = MC_FALSE;
    }

    return wErrorId;
}
#endif

/* Returns ErrorId if error, MC_NO_ERROR otherwise
 */
static MC_T_WORD CheckTrajParams(MC_T_AXIS_REF *pAxis, MC_T_REAL fVelocity, MC_T_REAL fAcceleration, MC_T_REAL fDeceleration, MC_T_REAL fJerk)
{
   if(fVelocity > pAxis->fMaxVel)
   {
      return MC_ERR_VEL_OUT_OF_RANGE;
   }
   else if(fAcceleration > pAxis->fMaxAcc)
   {
      return MC_ERR_ACC_OUT_OF_RANGE;
   }
   else if(fDeceleration > pAxis->fMaxDec)
   {
      return MC_ERR_DEC_OUT_OF_RANGE;
   }
   else if(fJerk > pAxis->fMaxJerk)
   {
      return MC_ERR_JERK_OUT_OF_RANGE;
   }

   return MC_NO_ERROR;
}

static MC_T_WORD HandleBlending(MC_T_AXIS_REF *pAxis,
                                MC_BUFFERED_FB_T *pMcfb,
                                EC_T_DWORD dwBufferMode,
                                MC_T_REAL  fVelocity,
                                MC_T_REAL  fEndVelocity,
                                MC_T_BUFFERENTRY**  ppPrevFb)
{
   MC_T_BUFFERENTRY*   pPrevFb = EC_NULL;
   MC_T_WORD wErrorId;
   MC_T_BOOL bDoRecalc = MC_FALSE;

   /* put FB into buffer */
   wErrorId = MC_BufferPut(pAxis, pMcfb, fEndVelocity, &pPrevFb);
   if (wErrorId != MC_NO_ERROR) return wErrorId;

   if((pPrevFb != EC_NULL) && (pPrevFb->pFB != EC_NULL) &&
      ((pPrevFb->pFB->wFbID == MCFB_ID_MOVE_ABSOLUTE) || (pPrevFb->pFB->wFbID == MCFB_ID_MOVE_RELATIVE) ||
      (pPrevFb->pFB->wFbID == MCFB_ID_MOVE_CONTINUOUS_ABSOLUTE) || (pPrevFb->pFB->wFbID == MCFB_ID_MOVE_CONTINUOUS_RELATIVE)))
   {
      /* handle the different blending modes */
      switch (dwBufferMode)
      {
      case MC_BLENDING_LOW:
         {
            if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_ABSOLUTE)
            {
               MC_MOVE_ABSOLUTE_T*     pPrevMcfb = (MC_MOVE_ABSOLUTE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = EC_MIN(pPrevMcfb->fVelocity, fVelocity);
            }
            else if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_RELATIVE)
            {
               MC_MOVE_RELATIVE_T*     pPrevMcfb = (MC_MOVE_RELATIVE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = EC_MIN(pPrevMcfb->fVelocity, fVelocity);
            }
            else if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_CONTINUOUS_ABSOLUTE)
            {
               MC_MOVE_CONT_ABSOLUTE_T*     pPrevMcfb = (MC_MOVE_CONT_ABSOLUTE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = EC_MIN(pPrevMcfb->fVelocity, fVelocity);
            }
            else if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_CONTINUOUS_RELATIVE)
            {
               MC_MOVE_CONT_RELATIVE_T*     pPrevMcfb = (MC_MOVE_CONT_RELATIVE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = EC_MIN(pPrevMcfb->fVelocity, fVelocity);
            }
            break;
         }
      case MC_BLENDING_PREVIOUS:
      case MC_BLENDING_PREVIOUS_STOP:
         {
            if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_ABSOLUTE)
            {
               MC_MOVE_ABSOLUTE_T*     pPrevMcfb = (MC_MOVE_ABSOLUTE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = pPrevMcfb->fVelocity;
            }
            else if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_RELATIVE)
            {
               MC_MOVE_RELATIVE_T*     pPrevMcfb = (MC_MOVE_RELATIVE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = pPrevMcfb->fVelocity;
            }
            else if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_CONTINUOUS_ABSOLUTE)
            {
               MC_MOVE_CONT_ABSOLUTE_T*     pPrevMcfb = (MC_MOVE_CONT_ABSOLUTE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = pPrevMcfb->fVelocity;
            }
            else if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_CONTINUOUS_RELATIVE)
            {
               MC_MOVE_CONT_RELATIVE_T*     pPrevMcfb = (MC_MOVE_CONT_RELATIVE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = pPrevMcfb->fVelocity;
            }
            break;
         }
      case MC_BLENDING_NEXT:
      case MC_BLENDING_NEXT_STOP:
         {
            bDoRecalc = MC_TRUE;
            pPrevFb->fEndVel   = fVelocity;
            break;
         }
      case MC_BLENDING_HIGH:
      case MC_BLENDING_HIGH_STOP:
         {
            if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_ABSOLUTE)
            {
               MC_MOVE_ABSOLUTE_T*     pPrevMcfb = (MC_MOVE_ABSOLUTE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = EC_MAX(pPrevMcfb->fVelocity, fVelocity);
            }
            else if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_RELATIVE)
            {
               MC_MOVE_RELATIVE_T*     pPrevMcfb = (MC_MOVE_RELATIVE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = EC_MAX(pPrevMcfb->fVelocity, fVelocity);
            }
            else if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_CONTINUOUS_ABSOLUTE)
            {
               MC_MOVE_CONT_ABSOLUTE_T*     pPrevMcfb = (MC_MOVE_CONT_ABSOLUTE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = EC_MAX(pPrevMcfb->fVelocity, fVelocity);
            }
            else if(pPrevFb->pFB->wFbID == MCFB_ID_MOVE_CONTINUOUS_RELATIVE)
            {
               MC_MOVE_CONT_RELATIVE_T*     pPrevMcfb = (MC_MOVE_CONT_RELATIVE_T*)pPrevFb->pFB;
               bDoRecalc = MC_TRUE;
               pPrevFb->fEndVel = EC_MAX(pPrevMcfb->fVelocity, fVelocity);
            }
            break;
         }
      }
   }

   if(ppPrevFb != EC_NULL)  *ppPrevFb = bDoRecalc ? pPrevFb : EC_NULL;
   return wErrorId;
}

/***************************************************************************************************/
/**
\brief  Set Error ID for Axis

* \return N/A
*/
void MC_T_AXIS_REF::SetAxisError(MC_T_WORD wErrorIDParam)
{
    this->wErrorID = wErrorIDParam;
    if (wErrorIDParam == MC_NO_ERROR)
    {
        if (this->eProfile == MC_T_AXIS_PROFILE_SERCOS
            && (this->wStatusWord & DRV_SER_STAT_FAULT))
        {
            this->bDriveErrorAckReq = MC_TRUE;
        }

        this->bError = MC_FALSE;
    }
    else
    {
        this->bError = MC_TRUE;
    }
}

void MC_POWER_T::InitAxis()
{
    /* calculate conversion factors */
    pAxis->dwCalcIncPerMM = EC_MAX(1, pAxis->dwIncPerMM*pAxis->lIncFactor);
    if ((pAxis->eOperationMode == OPMODE_PP) || (pAxis->eOperationMode == OPMODE_HOMING))
    {
        // In position profile mode (homing as well) there is no parameter conversion
        // for velocity, acceleration and jerk
        pAxis->fVelToInc = 1;
        pAxis->fAccToInc = 1;
        pAxis->fJerkToInc = 1;
    }
    else
    {
        pAxis->fVelToInc = (MC_T_REAL)((MC_T_POS_CALC)pAxis->dwCalcIncPerMM * pAxis->dwCycleTime / 1E6);
        pAxis->fAccToInc = (MC_T_REAL)(pAxis->fVelToInc  * pAxis->dwCycleTime / 1E6);
        pAxis->fJerkToInc = (MC_T_REAL)(pAxis->fAccToInc  * pAxis->dwCycleTime / 1E6);
    }

   pAxis->lVeltoAcc = EC_MAX((MC_T_INT)(1E6 / pAxis->dwCycleTime), 1);

   /* calculate maximum values */
   pAxis->fMaxDistance = ((sizeof(MC_T_POS_CALC) == 4) ? EC_INT32_MAX : EC_INT64_MAX) / (MC_T_REAL)pAxis->dwCalcIncPerMM;
   pAxis->fMaxVel  =  LONG_MAX / pAxis->fVelToInc;
   pAxis->fMaxAcc  =  LONG_MAX / pAxis->fAccToInc;
   pAxis->fMaxDec  =  LONG_MAX / pAxis->fAccToInc;
   pAxis->fMaxJerk =  LONG_MAX / pAxis->fJerkToInc;

   /* Check if Bit 12: Drive follows the command value is supported */
   /* pAxis->bDriveFollowBitSupp = pAxis->CheckDrvFollowBitSupported(pAxis->wProfileOperationMode); */

   MC_BufferClear(pAxis);          /* initialize FB buffer queue */
}

void MC_POWER_T::DS402StateMachine()
{
   MC_T_BOOL bEnableLocal = this->bEnable && this->bEnable_Positive && this->bEnable_Negative;
   EC_T_WORD wStatus = pAxis->wStatusWord;

   switch(pAxis->eDS402State)
   {
   default:
   case DRV_DEV_STATE_NOT_READY:           /* Not ready to switch on */
      if(bEnable)
      {
         InitAxis();

         /* check operation mode */
         if (pAxis->IsOpModeValid(pAxis->wProfileOperationMode))
         {
            if(((wStatus & 0x4F) == 0) || ((wStatus & 0x4F) == DRV_STAT_SWITCH_ON_DIS))
            {
               pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
            }
            else if(wStatus & DRV_STAT_FAULT)
            {
               pAxis->eDS402State = DRV_DEV_STATE_MALFCT_REACTION;
               pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
               pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
            }
            else if((wStatus & DRV_STAT_RDY_SWITCH_ON) == DRV_STAT_RDY_SWITCH_ON)
            {
               pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
            }
         }
         else
         {
            this->bError = MC_TRUE;
            this->wErrorID = MC_ERR_INVALID_OPERATION_MODE;
         }
      }
      else
      {
         /* The ‘Error’ outputs are reset with the falling edge of ‘Enable’ as soon as possible */
         this->bError = MC_FALSE;
      }
      break;

   case DRV_DEV_STATE_SWITCHON_DIS:        /* Optional state: Switch on disabled     */
      if(bEnableLocal)
      {
         InitAxis();

         if((wStatus & 0x4F) == DRV_STAT_SWITCH_ON_DIS)
         {
            pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_SHUTDOWN);
            pAxis->eDS402State = DRV_DEV_STATE_READY_TO_SWITCHON;
         }
         else if((wStatus & 0x4F) == DRV_STAT_RDY_SWITCH_ON)
         {
            pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_SHUTDOWN);
            pAxis->eDS402State = DRV_DEV_STATE_READY_TO_SWITCHON;
         }
         else if((wStatus & 0x69) == (DRV_STAT_QUICK_STOP | DRV_STAT_RDY_SWITCH_ON))
         {
            pAxis->eDS402State = DRV_DEV_STATE_SWITCHED_ON;
         }
         else if(wStatus & DRV_STAT_FAULT)
         {
            pAxis->eDS402State = DRV_DEV_STATE_MALFCT_REACTION;
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
            pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
         }
      }
      else
      {
         pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_DIS_VOLTAGE);
         /* The ‘Error’ outputs are reset with the falling edge of ‘Enable’ as soon as possible */
         this->bError = MC_FALSE;

         if(((wStatus & 0x4F) == 0) || ((wStatus & 0x4F) == DRV_STAT_RDY_SWITCH_ON) || ((wStatus & 0x4F) == DRV_STAT_SWITCH_ON_DIS))
         {
             this->bStatus    = MC_FALSE;
         }
      }
      break;

   case DRV_DEV_STATE_READY_TO_SWITCHON:   /* Ready to switch on    */
      if(bEnableLocal)
      {
         if((wStatus & 0x69) == (DRV_STAT_QUICK_STOP | DRV_STAT_RDY_SWITCH_ON))
         {
            pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_SWITCHON);
            pAxis->eDS402State = DRV_DEV_STATE_SWITCHED_ON;
         }
      }
      else
      {
         /* wait until drive is ready for lower state: DRV_STAT_SWITCHED_ON has to be cleared */
         if ((wStatus & 0x6F) == (DRV_STAT_QUICK_STOP  | DRV_STAT_RDY_SWITCH_ON))
         {
             pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_DIS_VOLTAGE);
             pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
         }
      }
      if(pAxis->wProfileOperationMode == DRV_MODE_OP_INTER_POS)
      {
         pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~DRV_CTRL_INTER_POS_ENA);       /* disable interpolation */
      }
      break;

   case DRV_DEV_STATE_SWITCHED_ON:         /* Switched on */
      if(bEnableLocal)
      {
         if((wStatus & 0x6B) == (DRV_STAT_QUICK_STOP | DRV_STAT_SWITCHED_ON | DRV_STAT_RDY_SWITCH_ON))
         {
            pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_ENA_OPERATION);

            MC_DriveSetTargetPosition(pAxis, pAxis->lActualPosition);
            MC_BufferClear(pAxis);          /* initialize FB buffer queue */
                                            
            pAxis->SetAxisError(MC_NO_ERROR);   /* clear all errors */

            pAxis->eDS402State = DRV_DEV_STATE_OP_ENABLED;
         }
      }
      else
      {
         /* wait until drive is ready for lower state: DRV_STAT_OP_ENABLED has to be cleared */
         if ((wStatus & 0x6F) == (DRV_STAT_QUICK_STOP  | DRV_STAT_SWITCHED_ON | DRV_STAT_RDY_SWITCH_ON))
         {
            pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_SHUTDOWN);
            pAxis->eDS402State = DRV_DEV_STATE_READY_TO_SWITCHON;
         }

         /* check for drive fault */
         if (wStatus & DRV_STAT_FAULT)
         {
            this->bError = MC_TRUE;
            pAxis->eDS402State = DRV_DEV_STATE_MALFCT_REACTION;
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
            pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
         }
      }
      break;

   case DRV_DEV_STATE_OP_ENABLED:          /* Operation enabled  */
      if(bEnableLocal)
      {
         if((wStatus & 0x6F) == (DRV_STAT_QUICK_STOP | DRV_STAT_OP_ENABLED | DRV_STAT_SWITCHED_ON | DRV_STAT_RDY_SWITCH_ON))
         {
            /* rising edge ? */
            if(!this->bStatus)
            {
               /* set initial axis state */
               pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
               MC_InitMovement(&pAxis->oMove, MC_FALSE, 0);
               pAxis->oMove.lCommandVel = pAxis->oMove.lCommandAcc = 0;
            }

            /* set all flags to enabled state */
            this->bStatus         = MC_TRUE;
            this->bError          = MC_FALSE;
            this->wErrorID        = MC_NO_ERROR;
            if(pAxis->wProfileOperationMode == DRV_MODE_OP_INTER_POS)
            {
               pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord | DRV_CTRL_INTER_POS_ENA);       /* enable interpolation */
            }

            /* check for following error */
            if (pAxis->wStatusWord & DRV_STAT_FOLLOW_ERR)
            {
               this->wErrorID = MC_ERR_FOLLOWING;
               this->bError   = MC_TRUE;
               this->bStatus  = MC_FALSE;
            }
         }
         else if(this->bStatus)
         {
             if ((pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STOPPING) && ((pAxis->wControlWord & DRV_CTRL_CMD_QUICK_STOP) == 0))
             {
                 pAxis->eDS402State = DRV_DEV_STATE_QUICK_STOP;
             }
             else
             {
                 /* quick stop in progress ? */
                 if((wStatus & 0x6F) == (DRV_STAT_OP_ENABLED | DRV_STAT_SWITCHED_ON | DRV_STAT_RDY_SWITCH_ON))
                 {
                     pAxis->eDS402State = DRV_DEV_STATE_QUICK_STOP;
                     pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;
                 }
                 else
                 {
                     /* unexpected change in status word */
                     this->bStatus = MC_FALSE;
                     this->bError = MC_TRUE;
                     this->wErrorID = MC_ERR_INVALID_STATUS_WORD;
                     if((wStatus & 0x4F) == DRV_STAT_SWITCH_ON_DIS)
                     {
                         pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
                     }
                     /* axis fault ? */
                     if (wStatus & DRV_STAT_FAULT)
                     {
                         pAxis->eDS402State = DRV_DEV_STATE_MALFCT_REACTION;
                         pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
                         pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
                     }
                 }
             }
         }
         else
         {
             /* transition 9: Disable voltage command from control device or local signal ? */
             if((wStatus & 0x4F) == DRV_STAT_SWITCH_ON_DIS)
             {
                 pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
             }

             /* axis fault */
             if (pAxis->wStatusWord & DRV_STAT_FAULT)
             {
                 this->bError   = MC_TRUE;
                 this->wErrorID = MC_ERR_INVALID_STATUS_WORD;
                 pAxis->eDS402State = DRV_DEV_STATE_MALFCT_REACTION;
                 pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
                 pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
             }
         }
      }
      else
      {
         pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_DIS_OPERATION);
         pAxis->eDS402State = DRV_DEV_STATE_SWITCHED_ON;
      }
      break;

   case DRV_DEV_STATE_QUICK_STOP:          /* Optional state: Quick stop active  */
      if((wStatus & 0x6F) == (DRV_STAT_OP_ENABLED | DRV_STAT_SWITCHED_ON | DRV_STAT_RDY_SWITCH_ON))
      {
         /* stay in this state */
      }
      else
      {
          pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
          this->bStatus = MC_FALSE;
      }
      break;

   case DRV_DEV_STATE_MALFCT_REACTION:     /* Malfunction reaction active */
       pAxis->oMove.lNextMoveCorrection = 0;
       MC_InitMovement(&pAxis->oMove, MC_FALSE, 0);
       pAxis->oMove.lCommandVel = pAxis->oMove.lCommandAcc = 0;
       MC_DriveSetTargetPosition(pAxis, pAxis->lActualPosition);

       pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~DRV_CRTL_FAULT_RESET);
       if((wStatus & 0xF) == (DRV_STAT_FAULT | DRV_STAT_OP_ENABLED | DRV_STAT_SWITCHED_ON | DRV_STAT_RDY_SWITCH_ON))
       {
           bFaultReaction = MC_TRUE;
           /* stay in this state */
       }
       else
       {
           bFaultReaction = MC_FALSE;
           pAxis->eDS402State = DRV_DEV_STATE_MALFUNCTION;
       }
       break;

   case DRV_DEV_STATE_MALFUNCTION:         /* Malfunction                 */
      if(bEnableLocal)
      {
          pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord | DRV_CRTL_FAULT_RESET);
          pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
          this->bError          = MC_FALSE;
      }
      this->bStatus = MC_FALSE;
      break;
   }

    if (pAxis->eLastDS402State != pAxis->eDS402State)
    {
        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "Axis=%4d: DS402 Drive State %d -> %d  StatusWord=0x%x  ControlWord=0x%x\n", 
            pAxis->wStationAddress, pAxis->eLastDS402State, pAxis->eDS402State, wStatus, pAxis->wControlWord));
        pAxis->eLastDS402State = pAxis->eDS402State;
    }
}

MC_T_BOOL MC_POWER_T::SercosSwitchToOp()
{
   MC_T_BOOL bEnableLocal = this->bEnable && this->bEnable_Positive && this->bEnable_Negative;

   if (!bEnableLocal) return MC_TRUE;

   /* check operation mode */
   if (!pAxis->IsOpModeValid(pAxis->wProfileOperationMode))
   {
      this->bError = MC_TRUE;
      this->wErrorID = MC_ERR_INVALID_OPERATION_MODE;
      return false;
   }

   // Switch drive to operational state
   pAxis->wControlWord |= DRV_SER_CRTL_ENABLED | DRV_SER_CRTL_ON | DRV_SER_CRTL_HLT;
   InitAxis();

   return MC_TRUE;
}

void MC_POWER_T::SercosStateMachine()
{
   EC_T_WORD  wStatus = pAxis->wStatusWord;
   MC_T_BOOL  bEnableLocal = this->bEnable && this->bEnable_Positive && this->bEnable_Negative;
   MC_T_SERCOS_STATE lastSercosState = pAxis->eSercosState;

   switch(pAxis->eSercosState)
   {
   case DRV_SER_STATE_NOT_READY:
      {
         if (wStatus & DRV_SER_STAT_FAULT)
         {
            pAxis->eSercosState = DRV_SER_STATE_MALFUNCTION;
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
            pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
            this->bStatus = MC_FALSE;
            break;
         }

         /* DRV_SER_STATE_NOT_READY is the initial state. We can reach all other states from here */
         if ((wStatus & DRV_SER_STAT_OPMASK) == DRV_SER_STAT_OP_NOT_READY)
         {
            /* stay in this state */
         }
         else if ((wStatus & DRV_SER_STAT_OPMASK) == DRV_SER_STAT_OP_READY_TO_SWITCHON)
         {
            pAxis->eSercosState = DRV_SER_STATE_READY_TO_SWITCHON;
         }
         else if ((wStatus & DRV_SER_STAT_OPMASK) == DRV_SER_STAT_OP_SWITCHED_ON)
         {
            pAxis->eSercosState = DRV_SER_STATE_SWITCHED_ON;
         }
         else if ((wStatus & DRV_SER_STAT_OPMASK) == DRV_SER_STAT_OP_ENABLED)
         {
            pAxis->eSercosState = DRV_SER_STATE_OP_ENABLED;
         }
         else
         {
            pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
         }

         break;
      }
   case DRV_SER_STATE_READY_TO_SWITCHON:
      {
         if (wStatus & DRV_SER_STAT_FAULT)
         {
            pAxis->eSercosState = DRV_SER_STATE_MALFUNCTION;
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
            pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
            this->bStatus = MC_FALSE;
            break;
         }

         if ((wStatus & DRV_SER_STAT_OPMASK) == DRV_SER_STAT_OP_READY_TO_SWITCHON)
         {
            /* Stay in this state */
         }
         else if ((wStatus & DRV_SER_STAT_OPMASK) == DRV_SER_STAT_OP_SWITCHED_ON)
         {
            pAxis->eSercosState = DRV_SER_STATE_SWITCHED_ON;
         }
         else
         {
            pAxis->eSercosState = DRV_SER_STATE_NOT_READY;
            pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
         }

         break;
      }
   case DRV_SER_STATE_SWITCHED_ON:
      {
         if (wStatus & DRV_SER_STAT_FAULT)
         {
            pAxis->eSercosState = DRV_SER_STATE_MALFUNCTION;
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
            pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
            this->bStatus = MC_FALSE;
            break;
         }

         if ((pAxis->wControlWord & DRV_SER_CRTLMASK) == 0)
         {
            SercosSwitchToOp();
         }

         if ((wStatus & DRV_SER_STAT_OPMASK) == DRV_SER_STAT_OP_SWITCHED_ON)
         {
            /* Stay in this state */
         }
         else if ((wStatus & DRV_SER_STAT_OPMASK) == DRV_SER_STAT_OP_ENABLED)
         {
            pAxis->eSercosState = DRV_SER_STATE_OP_ENABLED;
         }
         else if ((wStatus & DRV_SER_STAT_OPMASK) == DRV_SER_STAT_OP_READY_TO_SWITCHON)
         {
            /* Power stage down / Leistung wurde abgeschaltet */
            pAxis->eSercosState = DRV_SER_STATE_READY_TO_SWITCHON;
         }
         else
         {
            pAxis->eSercosState = DRV_SER_STATE_NOT_READY;
            pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
         }
         break;
      }
   case DRV_SER_STATE_OP_ENABLED:
      {
         if (wStatus & DRV_SER_STAT_FAULT)
         {
            pAxis->eSercosState = DRV_SER_STATE_MALFUNCTION;
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
            pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
            this->bStatus = MC_FALSE;
            break;
         }

         if (bEnableLocal)
         {
            if ((wStatus & DRV_SER_STAT_OPMASK) == DRV_SER_STAT_OP_ENABLED)
            {
               if (!this->bStatus)
               {
                  MC_DriveSetTargetPosition(pAxis, pAxis->lActualPosition);
                  EcLogMsg(EC_LOG_LEVEL_VERBOSE, (pEcLogContext, EC_LOG_LEVEL_VERBOSE, "Axis=%4: MC_DriveSetTargetPosition = %d pMove->lConstantVel=%d\n", 
                      pAxis->wStationAddress, pAxis->lActualPosition, pAxis->oMove.lConstantVel));
                  MC_BufferClear(pAxis);          /* initialize FB buffer queue */
               }

               if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_DISABLED)      /* MC_Power note 2 */
               {
                  /* set initial axis state */
                  pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
               }

               /* set all flags to enabled state */
               this->bStatus         = MC_TRUE;
               this->bError          = MC_FALSE;
            }
            else if (this->bStatus)
            {
               /* unexpected change in status word */
               this->bStatus         = MC_FALSE;
               this->bError          = MC_TRUE;
               this->wErrorID        = MC_ERR_INVALID_STATUS_WORD;

               pAxis->eSercosState = DRV_SER_STATE_NOT_READY;
               pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
               pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
            }
         }
         else
         {
            /* FB disabled: Turn off power stage */
            pAxis->wControlWord &= ~DRV_SER_CRTLMASK;
            pAxis->eSercosState = DRV_SER_STATE_NOT_READY;

            this->bStatus         = MC_FALSE;
            this->bError          = MC_FALSE;
         }
         break;
      }
   case DRV_SER_STATE_MALFUNCTION:
      {
         pAxis->wControlWord &= ~DRV_SER_CRTLMASK;          /* Reset power stage */
         MC_InitMovement(&pAxis->oMove, MC_FALSE, 0);
         pAxis->oMove.lCommandVel = pAxis->oMove.lCommandAcc = 0;
         MC_DriveSetTargetPosition(pAxis, pAxis->lActualPosition);

         if ( (wStatus & DRV_SER_STAT_FAULT) == 0
            && (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_DISABLED || pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STAND_STILL) )
         {
            /* Error has been cleared */
            pAxis->eSercosState = DRV_SER_STATE_NOT_READY;
            EcLogMsg(EC_LOG_LEVEL_VERBOSE, (pEcLogContext, EC_LOG_LEVEL_VERBOSE, "MC_DriveSetTargetPosition = %d pMove->lConstantVel=%d\n", pAxis->lActualPosition, pAxis->oMove.lConstantVel));
         }
         break;
      }
   default:
      break;
   }

   if (lastSercosState != pAxis->eSercosState)
   {
       EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "SERCOS State '%s' -> '%s'\n",
           SercosState2Str(lastSercosState), SercosState2Str(pAxis->eSercosState)));
   }
}

void MC_POWER_T::SimuStateMachine()
{
   MC_T_BOOL  bEnableLocal = this->bEnable && this->bEnable_Positive && this->bEnable_Negative;

   pAxis->eSimuState = DRV_SIM_STATE_OP_ENABLED;

   if (bEnableLocal)
   {
      if (!this->bStatus)
      {
         InitAxis();
         MC_DriveSetTargetPosition(pAxis, pAxis->lActualPosition);
         EcLogMsg(EC_LOG_LEVEL_VERBOSE, (pEcLogContext, EC_LOG_LEVEL_VERBOSE, "MC_DriveSetTargetPosition = %d pMove->lConstantVel=%d\n", pAxis->lActualPosition, pAxis->oMove.lConstantVel));
         MC_BufferClear(pAxis);          /* initialize FB buffer queue */
      }

      if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_DISABLED)      /* MC_Power note 2 */
      {
         /* set initial axis state */
         pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
      }

      /* set all flags to enabled state */
      this->bStatus         = MC_TRUE;
      this->bError          = MC_FALSE;
   }
   else
   {
      /* FB disabled */
      this->bStatus         = MC_FALSE;
      this->bError          = MC_FALSE;
   }
}

/***************************************************************************************************/
/**
\brief  Power On Drive

The MC_Power FB switches the power of the drive of the axis defined by AXIS_REF on
or off. It is assigned to the group of administrative FBs. (3.1)

- The power for the drive is connected only if all three inputs have the value TRUE.
- Regardless of the status of the axis, this FB can be commanded and the command
  can be sent to the motion controller.


* \return N/A
*/
void MC_POWER_T::OnCycle()
{
    MC_T_BOOL               bEnableLocal = this->bEnable && this->bEnable_Positive && this->bEnable_Negative;
    EC_T_WORD               wCurrState = DEVICE_STATE_UNKNOWN;
    EC_T_WORD               wReqState;
    MC_T_WORD               wLastStatusWord = 0;            /* Status Word */
    MC_T_WORD               wLastControlWord = 0;           /* Control Word */
#ifdef DEBUG
    EC_T_INT                nTrueCnt = 0;
#endif

    /* get slave state */
    if (pAxis->eAxisType == MC_AXIS_TYPE_VIRTUAL)
    {
        wCurrState = DEVICE_STATE_OP; // Simulated axis
    }
    else if (pAxis->pEcatGetSlaveState)
    {
        pAxis->pEcatGetSlaveState(this->Axis->dwMasterInstanceID, pAxis->dwSlaveID, &wCurrState, &wReqState);
    }

    /* Get input data */
    wLastStatusWord = pAxis->wStatusWord;
    wLastControlWord = pAxis->wControlWord;
    pAxis->wStatusWord = 0;
    pAxis->lActualPosition = 0;

    if (wCurrState == DEVICE_STATE_SAFEOP || wCurrState == DEVICE_STATE_OP)
    {
        if (pAxis->eAxisType == MC_AXIS_TYPE_REAL_ALL
            && pAxis->pwPdStatusWord != MC_NULL
            && pAxis->pwPdControlWord != MC_NULL)
        {

            pAxis->wStatusWord = EC_GET_FRM_WORD(pAxis->pwPdStatusWord);
            pAxis->wControlWord = EC_GET_FRM_WORD(pAxis->pwPdControlWord);

            if (wLastStatusWord != pAxis->wStatusWord)
            {
                EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "Axis=%4d: Cycle=%8d: StatusWord 0x%04x -> 0x%04x\n", 
                    pAxis->wStationAddress, pAxis->dwCycleCount, wLastStatusWord, pAxis->wStatusWord));
            }
        }

        MC_DriveGetInputs(pAxis);
    }

    /* state machine, note 2  */
    if (!this->bStatus)
    {
        MC_AbortCommand(pAxis, MCFB_ID_POWER);
        if (!pAxis->bError)
        {
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISABLED; // Set inital state
        }
    }

    if (pAxis->eAxisType == MC_AXIS_TYPE_REAL_ALL)
    {
        /* Process drive state machine */
        switch (pAxis->eProfile)
        {
        case MC_T_AXIS_PROFILE_DS402:
        {
            if (wCurrState == DEVICE_STATE_OP)   DS402StateMachine();
            else   pAxis->eDS402State = DRV_DEV_STATE_NOT_READY;
            break;
        }
        case MC_T_AXIS_PROFILE_SERCOS:
        {
            if (wCurrState == DEVICE_STATE_OP)   SercosStateMachine();
            else   pAxis->eSercosState = DRV_SER_STATE_NOT_READY;
            break;
        }
        case MC_T_AXIS_PROFILE_NONE:
            this->bValid = MC_FALSE;
            this->bError = MC_TRUE;
            this->wErrorID = MC_ERR_NO_ECAT_PROFILE;
            break;
        }
    }
    else
    {
        SimuStateMachine();
    }

    if (bEnableLocal && !this->bError)
    {
        this->bValid = MC_TRUE;
    }
    else
    {
        this->bValid = MC_FALSE;
    }

    /* state machine, note 5 */
    if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_DISABLED)
    {
        if (bEnableLocal && this->bStatus)
        {
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
        }
    }

    /* save for future use */
    pAxis->bMcPowerStatus = this->bStatus;
    pAxis->bMcPowerEnable = bEnableLocal;

    if (this->bStatus && !this->bError)
    {
#ifdef EC_MOTION_SUPPORT_PP_MODE
        if (pAxis->eSetValSource == SETVAL_TRAJGEN_DRV)
        {
            MC_PPSm(pAxis);
        }
        else
#endif
        {
            /* Trajectory Generator */
            MC_T_MOVEMENT*  pMove = &pAxis->oMove;

            pMove->lTargetPosStep = 0;

            // calc next increment
#if defined(EC_MOTION_SUPPORT_MC_CAMMING) || defined(EC_MOTION_SUPPORT_MC_GEARING)
            if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_SYNC_MOTION)
            {
#ifdef EC_MOTION_SUPPORT_MC_CAMMING
                if ((pAxis->eSetValSource == SETVAL_MASTER_CAMM) && (pAxis->pSyncMasterAxis != EC_NULL))   /* check if a master axis is assigned */
                {
                    if (pAxis->pCamTableID != EC_NULL)   /* check if a CAM table is assigned */
                    {
                        MC_T_POS_CALC lTargetPosition = 0;
                        MC_T_REAL     fTargetPos;
                        MC_T_REAL     fMasterPos;
                        MC_T_BOOL     bValidSlavePos = EC_FALSE;

                        /* actual master pos */
                        fMasterPos = pAxis->pSyncMasterAxis->lCommandedPosition / (MC_T_REAL)pAxis->pSyncMasterAxis->dwCalcIncPerMM;

                        wErrorID = pAxis->pCamTableID->CalcSlavePos(fMasterPos, &fTargetPos, &bValidSlavePos);
                        if (wErrorID != MC_NO_ERROR)
                        {
                            this->bError = MC_TRUE;
                        }
                        else if (bValidSlavePos)     /* only update slave position if we got a valid position value */
                        {
                            lTargetPosition = (MC_T_POS_CALC)(fTargetPos * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
                            pMove->lTargetPosStep = (MC_T_POSITION)(lTargetPosition - pAxis->lCommandedPosition);

                            pAxis->dwDirection = (pMove->lTargetPosStep > 0) ? MC_DIR_POSITIVE : MC_DIR_NEGATIVE;
                            pMove->lTargetPosStep = labs(pMove->lTargetPosStep);

                            /* calculate commanded lCommandVel, lCommandAcc, lCommandJerk */
                            pMove->lCommandVel = pMove->lConstantVel = pMove->lTargetPosStep;
                            pMove->lCommandAcc = pMove->lTargetPosStep - pMove->lLastTargetPosStep;
                        }
                    }
                    else
                    {
                        this->wErrorID = MC_ERR_CAM_TABLE_ID_INVALID;
                        this->bError = MC_TRUE;
                    }
                }
#endif
#ifdef EC_MOTION_SUPPORT_MC_GEARING
                else if ((pAxis->eSetValSource == SETVAL_MASTER_GEAR) && (pAxis->pSyncMasterAxis != EC_NULL))   /* check if a master axis is assigned */
                {
                    if (pAxis->oGearingData.dwRatioDenominator != 0)   /* check if a gearing parameter is assigned */
                    {
                        MC_T_REAL     fSlaveVel;
                        MC_T_REAL     fMasterVel;
                        MC_T_VELACC   lMasterVelInc;

                        /* get velocity of master axis */
                        if (pAxis->oGearingData.eMasterValueSource == MC_SOURCE_SET_VALUE)
                        {
                            lMasterVelInc = pAxis->pSyncMasterAxis->oMove.lCommandVel;
                            /* lCommandVel is always positive, therefore check dwDirection */
                            if (pAxis->pSyncMasterAxis->dwDirection == MC_DIR_NEGATIVE) lMasterVelInc = -lMasterVelInc;
                        }
                        else
                        {
                            lMasterVelInc = pAxis->pSyncMasterAxis->lActualVelocity;
                        }
                        fMasterVel = lMasterVelInc / pAxis->pSyncMasterAxis->fVelToInc;

                        /* calc velocity of slave axis */
                        fSlaveVel = fMasterVel * pAxis->oGearingData.nRatioNumerator / (MC_T_REAL)pAxis->oGearingData.dwRatioDenominator;
                        fSlaveVel = fSlaveVel + pMove->fSlaveVelKorrection;
                        pMove->lTargetPosStep = (MC_T_POSITION)(fSlaveVel * pAxis->fVelToInc);

                        /* calculate error due to increments and consider it in the next cycle */
                        pMove->fSlaveVelKorrection = fSlaveVel - (pMove->lTargetPosStep / pAxis->fVelToInc);

                        if (fSlaveVel >= 0)
                        {
                            pAxis->dwDirection = MC_DIR_POSITIVE;
                        }
                        else
                        {
                            pAxis->dwDirection = MC_DIR_NEGATIVE;
                            pMove->lTargetPosStep = -pMove->lTargetPosStep;
                        }

                        /* calculate commanded lCommandVel, lCommandAcc */
                        pMove->lCommandVel = pMove->lConstantVel = pMove->lTargetPosStep;
                        pMove->lCommandAcc = pMove->lTargetPosStep - pMove->lLastTargetPosStep;
                    }
                    else
                    {
                        this->wErrorID = MC_ERR_GEAR_PARA_INVALID;
                        this->bError = MC_TRUE;
                    }
                }
#endif
                else
                {
                    this->wErrorID = MC_ERR_MASTER_AXIS_INVALID;
                    this->bError = MC_TRUE;
                }
            }
#endif
            if (pAxis->eSetValSource == SETVAL_TRAJGEN_ECM)
            {
                MC_TrajNextSetPoint(pMove);

                if ((pMove->lTotalDist != 0) && ((pMove->lEndVel == 0) || (pMove->lRemainDist >= pMove->lTargetPosStep)))
                {
                    pMove->lRemainDist = pMove->lRemainDist - pMove->lTargetPosStep;
                    OsDbgAssert(pMove->lRemainDist >= 0);
                }
                else if (!pMove->bEndless && (pMove->lEndVel != 0))
                {
                    if (pMove->lRemainDist != 0)
                    {
                        /* terminate current move and save the remain dist for next move */
                        pMove->lNextMoveCorrection = pMove->lRemainDist;
                        pMove->lRemainDist = 0;
                        pMove->lRemainStep = 0;
                        pMove->bRemainStepCalc = MC_FALSE;
                        pMove->lVelRemainder[0] = pMove->lVelRemainder[1] = 0;
                    }
                }

                if (pMove->lRemainDist == 0)
                {
                    if (pMove->lRemainStep != 0) EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "Trajectory error. RemainDist=0 and RemainStep=%d\n", pMove->lRemainStep));
                    OsDbgAssert(pMove->lRemainStep == 0);
                }
            }
#ifdef EC_MOTION_SUPPORT_MC_POSITION_PROFILE
            else if(pMove->pProfile != EC_NULL)
            {
                /* Profile mode */
                MC_T_POS_CALC lTargetPos;

                lTargetPos = (MC_T_POS_CALC)(pMove->pProfile->pSpline->get_result(pMove->fTimeInSpline) * pAxis->dwCalcIncPerMM);
                pMove->lTargetPosStep = (MC_T_POSITION)(lTargetPos - pAxis->lCommandedPosition);

                //OsDbgMsg("time=%4d  Spline=%d  lTargetPos=%ld  lTargetPosStep=%ld\n",
                //   (EC_T_INT)(pMove->fTimeInSpline*1000), (EC_T_INT)lTargetPos, (MC_T_POSITION)lTargetPos, pMove->lTargetPosStep);

                /* calculate commanded lCommandVel, lCommandAcc, lCommandJerk */
                pMove->lCommandVel = pMove->lTargetPosStep;
                pMove->lCommandAcc = pMove->lTargetPosStep - pMove->lLastTargetPosStep;

                /* check if done ? */
                if(pMove->fTimeInSpline < pMove->fTotalTime)
                {
                    pMove->fTimeInSpline += pAxis->dwCycleTime; /*pMove->fSplineStep */;
                }
            }
#endif
            else
            {
                pMove->lCommandVel = pMove->lTargetPosStep;
                pMove->lCommandAcc = pMove->lTargetPosStep - pMove->lLastTargetPosStep;
            }

            /* calculate commanded jerk */
            pMove->lCommandJerk = pMove->lCommandAcc - pMove->lLastCommandAcc;
            pMove->lLastCommandAcc = pMove->lCommandAcc;

            /* not supported
            if (pAxis->bDriveFollowBitSupp)
            {
              if((pAxis->wStatusWord & DRV_STAT_FOLLOW_CMD_VALUE) == 0)
                  wErrorID = MC_ERR_NOT_FOLLOW_CMD_VALUE;
            } */

            /* update target position or velocity */
            MC_DriveSetTargetStep(pAxis, pMove->lTargetPosStep, pMove->lTargetVel, pMove->lTargetAcc);
            pMove->lLastTargetPosStep = pMove->lTargetPosStep;
#ifdef DEBUG
            for (EC_T_INT nIdx = STEP_BUFFER_SIZE - 1; nIdx > 0; nIdx--)
            {
                pMove->aStepBuffer[nIdx] = pMove->aStepBuffer[nIdx - 1];
            }
            pMove->aStepBuffer[0] = pMove->lTargetPosStep;
#endif
            pMove->lTargetPosStep = 0;
        } /* else (pAxis->bTrajectoryInDrive) */
    } /* if (this->bStatus && !this->bError) */

    /* update control word in process data output image */
    if (pAxis->eAxisType == MC_AXIS_TYPE_REAL_ALL)
    {
        if (wLastControlWord != pAxis->wControlWord)
        {
            EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "Axis=%4d: Cycle=%8d: ControlWord 0x%04x -> 0x%04x\n", 
                pAxis->wStationAddress, pAxis->dwCycleCount, wLastControlWord, pAxis->wControlWord));
        }
        if (pAxis->pwPdControlWord != EC_NULL)
        {
            EC_SET_FRM_WORD(pAxis->pwPdControlWord, pAxis->wControlWord);
        }
        if (pAxis->pbyPdModeOfOperation != EC_NULL)
        {
            *pAxis->pbyPdModeOfOperation = (EC_T_BYTE)pAxis->wProfileOperationMode;
        }
    }

    pAxis->dwCycleCount++;      /* for logging purpose only */

    /* check "output exclusivity" */
#ifdef DEBUG
    if (this->bEnable)
    {
        if (this->bValid) nTrueCnt++;
        if (this->bError) nTrueCnt++;
        OsDbgAssert(nTrueCnt <= 1);
    }
#endif

    if (pAxis->eLastPLCOpenState != pAxis->ePLCOpenState)
    {
        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "Axis=%4d: PLCOpen State '%s' -> '%s'\n", 
            pAxis->wStationAddress, PLCOpenStateText(pAxis->eLastPLCOpenState), PLCOpenStateText(pAxis->ePLCOpenState)));
        pAxis->eLastPLCOpenState = pAxis->ePLCOpenState;
    }
}

/* Returns error ID or MC_NO_ERROR if success.
 */
static MC_T_WORD CalcBrakeRamp(MC_T_AXIS_REF *pAxis, MC_T_REAL fDeceleration, MC_T_REAL fJerk)
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;
   MC_T_WORD wErrorID;

   pMove->lMaxDec =  (MC_T_VELACC)(fDeceleration * pAxis->fAccToInc);
   pMove->lMaxDec = EC_MIN(pMove->lMaxDec, pMove->lCommandVel);
   pMove->lMaxDec =  EC_MAX(pMove->lMaxDec, 1);

   if(fJerk != 0)
   {
      pMove->lJerk =  (MC_T_VELACC)(fJerk * pAxis->fJerkToInc);
      pMove->lJerk =  EC_MAX(pMove->lJerk, 1);
      pMove->lJerk =  EC_MIN(pMove->lJerk, pMove->lMaxDec);
   }
   else
   {
      pMove->lJerk = 0;
   }

   wErrorID = MC_TrajBreakDist(pMove);
   if(wErrorID == MC_NO_ERROR)
   {
      pMove->lTotalDist = pMove->lRemainDist = pMove->lSegDist5to7;
      pMove->lDoneDist  = pMove->dwDoneCycles = 0;
   }

   return wErrorID;
}

// Homing supported only in PP mode
#ifdef EC_MOTION_SUPPORT_PP_MODE
/********************************************************************************/
/** \brief MC_Home Controlled motion stop (3.2)

This Function Block commands the axis to perform the «search home» sequence.
The details of this sequence are manufacturer dependent and can be set by
the axis’ parameters. The ‘Position’ input is used to set the absolute position
when reference signal is detected. This Function Block completes at ‘Standstill’
if it was started in ‘Standstill’.
*/
void MC_HOME_T::StateTrans_Execute()
{
   if(pAxis == EC_NULL)
   {
      this->wErrorID = MC_ERR_INVALID_PARAMETER;
      this->ChangeState(FBS_ERROR);
      return;
   }
    /* handle the different blending modes */
    switch (this->dwBufferMode)
    {
    case MC_BLENDING_LOW:
    case MC_BLENDING_PREVIOUS:
    case MC_BLENDING_PREVIOUS_STOP:
    case MC_BLENDING_NEXT:
    case MC_BLENDING_NEXT_STOP:
    case MC_BLENDING_HIGH:
    case MC_BLENDING_HIGH_STOP:
    {
        this->wErrorID = MC_ERR_BUFFERMODE_NOT_SUPP;
        this->ChangeState(FBS_ERROR);

        OsDbgAssert(MC_FALSE);
        EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "%s: Error Buffermode%d not supported\n", GetName(), this->dwBufferMode));
        break;
    }
    default:
    {
        MC_T_BUFFERENTRY*   pPrevFb = EC_NULL;

        if (pAxis->ePLCOpenState != MC_PLCOPEN_STATE_STAND_STILL)
        {
            // Homing can be started only from 'Standstill' state
            this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
            this->ChangeState(FBS_ERROR);
            return;
        }

        /* put FB into buffer */
        this->wErrorID = MC_BufferPut(pAxis, this, 0, &pPrevFb);
        if (this->wErrorID != MC_NO_ERROR)
        {
            this->ChangeState(FBS_ERROR);
            break;
        }

        this->ChangeState(FBS_BUSY);
    }
    } // end switch
}

void MC_HOME_T::DoState_Busy()
{
    MC_T_BOOL    fbActive = false;

    this->wErrorID = MC_BufferActivateNext(pAxis, this, &fbActive);
    if (this->wErrorID == MC_NO_ERROR)
    {
        if (fbActive)
        {
            /* this FB is now active */
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_HOMING;
            pAxis->eOperationMode = OPMODE_HOMING;

            this->ChangeState(FBS_ACTIVE);
        }
    }
    else
    {
        this->ChangeState(FBS_ERROR);
    }
}

void MC_HOME_T::StateTrans_Active()
{
    MC_T_MOVEMENT*  pMove = &pAxis->oMove;

    if (pAxis->ePLCOpenState != MC_PLCOPEN_STATE_HOMING)
    {
        this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
        this->ChangeState(FBS_ERROR);
        return;
    }

    /* Request new profile */
    pMove->eProfileState = eProfileStatus_SwitchToHomingMode;
}

void MC_HOME_T::DoState_Active()
{
    MC_T_BOOL bDoneLocal = MC_FALSE;

    this->wErrorID = MC_CheckHomingDone(pAxis, &bDoneLocal);
    if (this->wErrorID != MC_NO_ERROR)
    {
        this->ChangeState(FBS_ERROR);
        return;
    }

    if (bDoneLocal)
    {
        if (bEnableSetPosition)
        {
            // Set homing position (absolute)
            MC_SetPosition(this->pAxis, this->fPosition, EC_FALSE);
        }
        else
        {
            // Disable position offset
            pAxis->lPosOffset = 0;
        }

        pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
        this->ChangeState(FBS_DONE);

        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: Exe=%d Busy=%d Active=%d Done=%d CmdAborted=%d Error=%d\n",
            GetName(), this->bExecute, this->bBusy, this->bActive, this->bDone, this->bCommandAborted, this->bError));
    }
}

MC_T_BOOL MC_HOME_T::StateMachine()
{
    MC_T_BOOL continueSm = MC_FALSE; // false if state machine should terminate, true to keep running.

    switch (this->fbState)
    {
    case FBS_IDLE:
    {
        if (this->bExecute)
        {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
                this->bError = MC_TRUE;
                continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
                this->bBusy = MC_TRUE;
                continueSm = MC_TRUE;
            }
        }
        break;
    }
    case FBS_BUSY:
    {
        DoState_Busy();
        if (this->fbState == FBS_ACTIVE)
        {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
        }

        if (this->fbState == FBS_ERROR)
        {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
        }
        break;
    }
    case FBS_ACTIVE:
    {
        DoState_Active();
        if (this->fbState == FBS_DONE)
        {
            this->bDone = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
        }
        else if (this->fbState == FBS_ERROR)
        {
            this->bError = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
        }
        break;
    }
    case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
    case FBS_ERROR:
    case FBS_DONE:
    {
        /* 2.4.1 General rules - Output status
        * Done, Error, ErrorID and CommandAborted are reset with the falling edge
        * of Execute.
        * It must be guaranted that the coresponding outputs are set for at least
        * one cycle...
        */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
        if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
        {
            this->bDone = MC_FALSE;
            this->bCommandAborted = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
        }
        break;
    }
    }

    /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
    OsDbgAssert(IsExclusive(this->bBusy, this->bDone, this->bError, this->bCommandAborted));
    /* ... If Execute is true, one of these outputs has to be true. */
    OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bDone, this->bError, this->bCommandAborted));

    return continueSm;
}

void MC_HOME_T::OnCycle()
{
    while (StateMachine());
}
#endif

/********************************************************************************/
/** \brief MC_Stop Controlled motion stop (3.3)

 This Function Block commands a controlled motion stop and transfers the axis to the state ‘Stopping’. It aborts any
 ongoing Function Block execution. While the axis is in state ‘Stopping’, no other FB can perform any motion on the
 same axis. After the axis has reached ‘Velocity’ zero, the ‘Done’ output is set to TRUE immediately. The axis remains
 in the state ‘Stopping’ as long as ‘Execute’ is still TRUE or ‘Velocity’ zero is not yet reached. As soon as
 ‘Done’ is SET and ‘Execute’ is FALSE the axis goes to state ‘Standstill’.
 */
void MC_STOP_T::StateTrans_Execute()
{
    MC_T_MOVEMENT*  pMove = &pAxis->oMove;

   if(pAxis == EC_NULL)
   {
      this->wErrorID = MC_ERR_INVALID_PARAMETER;
      this->ChangeState(FBS_ERROR);
      return;
   }
    /* rising edge */
#if defined(EC_MOTION_SUPPORT_MC_CAMMING) || defined(EC_MOTION_SUPPORT_MC_GEARING)
   pAxis->eSetValSource = pAxis->eSetValSourceOrg;
   pAxis->pSyncMasterAxis = EC_NULL;
#endif
    pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STOPPING;

    MC_AbortCommand(pAxis, MCFB_ID_STOP);     /* abort other MCFBs */

    // No brake ramp calculation in PP mode
    if(pAxis->eSetValSource == SETVAL_TRAJGEN_ECM)
    {
        /* calculate break distance */
        if (pMove->lCommandVel != 0)
        {
            this->wErrorID = CalcBrakeRamp(pAxis, this->fDeceleration, this->fJerk);
            if (this->wErrorID != MC_NO_ERROR)
            {
                this->ChangeState(FBS_ERROR);
                return;
            }
        }
    }

   EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: lCommandVel=%d lRemainDist=%d\n", GetName(), pMove->lCommandVel, pMove->lRemainDist));
   EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: Exe=%d Busy=%d Done=%d CmdAborted=%d Error=%d\n",
         GetName(), this->bExecute, this->bBusy, this->bDone, this->bCommandAborted, this->bError));

   this->ChangeState(FBS_BUSY);
}

void MC_STOP_T::DoState_Busy()
{
    OsDbgAssert(pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STOPPING);

    /* 3.3 CommandAborted - 'Command' is aborted by switching off power (only possibility to abort)
     */
    if (!pAxis->bMcPowerStatus)
    {
        this->ChangeState(FBS_COMMAND_ABORTED);
        return;
    }

    MC_T_BOOL bDoneLocal = MC_FALSE;
#ifdef EC_MOTION_SUPPORT_PP_MODE
    if (pAxis->eSetValSource == SETVAL_TRAJGEN_DRV)
    {
        // For PP mode
        this->wErrorID = MC_CheckTargetPosReachedTrajInDrive(pAxis, &bDoneLocal);
    }
    else
#endif
    {
        this->wErrorID = MC_CheckTargetPosReached(pAxis, &bDoneLocal);
    }

    if (this->wErrorID != MC_NO_ERROR)
    {
        this->ChangeState(FBS_ERROR);
        return;
    }

    if (bDoneLocal)
    {
        this->ChangeState(FBS_DONE);

        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: Exe=%d Busy=%d Done=%d CmdAborted=%d Error=%d\n",
            GetName(), this->bExecute, this->bBusy, this->bDone, this->bCommandAborted, this->bError));
    }
}

MC_T_BOOL MC_STOP_T::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP; // state machine, note 1; Reset in MC_Reset()
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bError = MC_TRUE;
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP; // state machine, note 1; Reset in MC_Reset()
            continueSm = MC_TRUE;
         }
         else if (this->fbState == FBS_DONE)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bDone = MC_TRUE;
            continueSm = MC_TRUE;
         }
         else if (this->fbState == FBS_COMMAND_ABORTED)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bCommandAborted = MC_TRUE;
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP; // Not specified.
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // Set by MC_Stop_DoState_Busy() if powerfail
   case FBS_ERROR:
   case FBS_DONE:
      {
         if (this->fbState == FBS_DONE && !this->bExecute)
         {
            /* As soon as ‘Done’ is SET and ‘Execute’ is FALSE the axis goes to state ‘Standstill’
             * state machine, note 6
             */
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
         }

         /* 2.4.1 General rules - Output status
          * Done, Error, ErrorID and CommandAborted are reset with the falling edge
          * of Execute.
          * It must be guaranted that the coresponding outputs are set for at least
          * one cycle...
          */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bDone = MC_FALSE;
            this->bCommandAborted = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         break;
      }
   default:
      break;
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->bDone, this->bError, this->bCommandAborted));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bDone, this->bError, this->bCommandAborted));

   return continueSm;
}

void MC_STOP_T::OnCycle()
{
   while (StateMachine());
}


/********************************************************************************/
/** \brief MC_Halt Controlled motion stop (3.4)

 This Function Block commands a controlled motion stop. The axis is moved to the state 'DiscreteMotion',
 until the velocity is zero. With the 'Done' output set, the state is transferred to 'Standstill'.
 */
void MC_HALT_T::DoState_Busy()
{
    MC_T_BOOL    fbActive = false;
#ifdef EC_MOTION_SUPPORT_MC_POSITION_PROFILE
    if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STOPPING && this->dwBufferMode == MC_ABORTING)
    {
        /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
         * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
         */
        this->wErrorID = MC_ERR_BUSY_STOPPING;
        this->ChangeState(FBS_ERROR);
        return;
    }
#endif
    OsDbgAssert(!(pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STOPPING && this->dwBufferMode == MC_ABORTING));

    if (this->dwBufferMode == MC_ABORTING)
    {
        MC_AbortCommand(pAxis, MCFB_ID_HALT);     /* abort other MCFBs */
    }

    this->wErrorID = MC_BufferActivateNext(pAxis, this, &fbActive);
    if (this->wErrorID == MC_NO_ERROR)
    {
        if (fbActive)
        {
            /* this FB is now active */
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISC_MOTION;
            this->ChangeState(FBS_ACTIVE);
        }
    }
    else
    {
        this->ChangeState(FBS_ERROR);
    }
}

void MC_HALT_T::StateTrans_Active()
{
    MC_T_MOVEMENT*  pMove = &pAxis->oMove;

    /* rising edge */
#if defined(EC_MOTION_SUPPORT_MC_CAMMING) || defined(EC_MOTION_SUPPORT_MC_GEARING)
    pAxis->eSetValSource = pAxis->eSetValSourceOrg;
    pAxis->pSyncMasterAxis = EC_NULL;
#endif


#ifdef EC_MOTION_SUPPORT_PP_MODE
    if (pAxis->eSetValSource == SETVAL_TRAJGEN_DRV)
    {
        /* Request halt */
        pMove->eProfileState = eProfileStatus_Halt;
    }
    else
#endif
    {
        if (pMove->lCommandVel == 0) return; // Brake velocity must be set

        this->wErrorID = CalcBrakeRamp(pAxis, this->fDeceleration, this->fJerk);
        if (this->wErrorID != MC_NO_ERROR)
        {
            this->ChangeState(FBS_ERROR);
            return;
        }
    }
}

void MC_HALT_T::DoState_Active()
{
    MC_T_BOOL bDoneLocal = MC_FALSE;

    if (!((pAxis->ePLCOpenState == MC_PLCOPEN_STATE_DISC_MOTION) ||
        (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STAND_STILL) ||
        (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_HOMING)))
    {
        this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
        this->ChangeState(FBS_ERROR);
        return;
    }

#ifdef EC_MOTION_SUPPORT_PP_MODE
    if (pAxis->eSetValSource == SETVAL_TRAJGEN_DRV)
    {
        // For PP mode
        this->wErrorID = MC_CheckTargetPosReachedTrajInDrive(pAxis, &bDoneLocal);
    }
    else
#endif
    {
        this->wErrorID = MC_CheckTargetPosReached(pAxis, &bDoneLocal);
    }

    if (this->wErrorID != MC_NO_ERROR)
    {
        this->ChangeState(FBS_ERROR);
        return;
    }

    if (bDoneLocal)
    {
#ifdef EC_MOTION_SUPPORT_PP_MODE
        if (pAxis->eSetValSource == SETVAL_TRAJGEN_DRV)
        {
            MC_DriveSetTargetPosition(pAxis, pAxis->lActualPosition);
        }
#endif
        pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
        this->ChangeState(FBS_DONE);

        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: Exe=%d Busy=%d Active=%d Done=%d CmdAborted=%d Error=%d\n",
            GetName(), this->bExecute, this->bBusy, this->bActive, this->bDone, this->bCommandAborted, this->bError));
    }
}

void MC_HALT_T::StateTrans_Execute()
{
   if(pAxis == EC_NULL)
   {
      this->wErrorID = MC_ERR_INVALID_PARAMETER;
      this->ChangeState(FBS_ERROR);
      return;
   }
    /* handle the different blending modes */
    switch (this->dwBufferMode)
    {
    case MC_BLENDING_LOW:
    case MC_BLENDING_PREVIOUS:
    case MC_BLENDING_PREVIOUS_STOP:
    case MC_BLENDING_NEXT:
    case MC_BLENDING_NEXT_STOP:
    case MC_BLENDING_HIGH:
    case MC_BLENDING_HIGH_STOP:
    {
        this->wErrorID = MC_ERR_BUFFERMODE_NOT_SUPP;
        this->ChangeState(FBS_ERROR);

        OsDbgAssert(MC_FALSE);
        EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "%s: Error Buffermode%d not supported\n", GetName(), this->dwBufferMode));
        break;
    }
    default:
    {
        MC_T_BUFFERENTRY*   pPrevFb = EC_NULL;

        /* put FB into buffer */
        this->wErrorID = MC_BufferPut(pAxis, this, 0, &pPrevFb);
        if (this->wErrorID != MC_NO_ERROR)
        {
            this->ChangeState(FBS_ERROR);
            break;
        }

        this->ChangeState(FBS_BUSY);
    }
    } // end switch
}

MC_T_BOOL MC_HALT_T::StateMachine()
{
    MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

    switch (this->fbState)
    {
    case FBS_IDLE:
    {
        if (this->bExecute)
        {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
                this->bError = MC_TRUE;
                continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
                this->bBusy = MC_TRUE;
                continueSm = MC_TRUE;
            }
        }
        break;
    }
    case FBS_BUSY:
    {
        DoState_Busy();
        if (this->fbState == FBS_ACTIVE)
        {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
        }

        if (this->fbState == FBS_ERROR)
        {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
        }
        break;
    }
    case FBS_ACTIVE:
    {
        DoState_Active();
        if (this->fbState == FBS_DONE)
        {
            this->bDone = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
        }
        else if (this->fbState == FBS_ERROR)
        {
            this->bError = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
        }
        break;
    }
    case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
    case FBS_ERROR:
    case FBS_DONE:
    {
        /* 2.4.1 General rules - Output status
         * Done, Error, ErrorID and CommandAborted are reset with the falling edge
         * of Execute.
         * It must be guaranted that the coresponding outputs are set for at least
         * one cycle...
         */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
        if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
        {
            this->bDone = MC_FALSE;
            this->bCommandAborted = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
        }
        break;
    }
    }

    /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
     * exclusive.
     */
    OsDbgAssert(IsExclusive(this->bBusy, this->bDone, this->bError, this->bCommandAborted));
    /* ... If Execute is true, one of these outputs has to be true. */
    OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bDone, this->bError, this->bCommandAborted));

    return continueSm;
}

void MC_HALT_T::OnCycle()
{
    while (StateMachine());
}


/********************************************************************************/
/** \brief Move to an absolute position (3.5)

Movement of the axis to an absolute position is commanded with the FB
MC_MoveAbsolute FB. The motion is always executed jerk-limited.
This value is valid for both acceleration and deceleration.

*/
void MC_MOVE_ABSOLUTE_T::StateTrans_Execute()
{
   MC_T_BUFFERENTRY*    pPrevFb;

   if(pAxis == EC_NULL)
   {
      this->wErrorID = MC_ERR_INVALID_PARAMETER;
      this->ChangeState(FBS_ERROR);
      return;
   }

   /* check if move(input) parameters are valid */
   this->wErrorID = CheckTrajParams(pAxis, this->fVelocity, this->fAcceleration, this->fDeceleration, this->fJerk);
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->wErrorID = HandleBlending(pAxis, this, this->dwBufferMode, this->fVelocity, 0, &pPrevFb);
   /* do we have to recalc the current movement ? */
   if ((this->wErrorID == MC_NO_ERROR) && (pPrevFb != EC_NULL))
   {
       MC_T_REAL fDistance = ((MC_T_REAL)pAxis->oMove.lTargetPosition/pAxis->dwCalcIncPerMM) - this->fPosition;

       fDistance = fabs(fDistance);
       this->wErrorID = BlendingRecalcMove(pPrevFb, fDistance);
   }
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->ChangeState(FBS_BUSY);
}

void MC_MOVE_ABSOLUTE_T::DoState_Busy()
{

   switch (pAxis->ePLCOpenState)
   {
   case MC_PLCOPEN_STATE_STAND_STILL:
   case MC_PLCOPEN_STATE_DISC_MOTION:
   case MC_PLCOPEN_STATE_CONT_MOTION:
      {
         MC_T_BOOL fbActive = false;
         if(this->dwBufferMode == MC_ABORTING)
         {
            MC_AbortCommand(pAxis, MCFB_ID_MOVE_ABSOLUTE);     /* abort other MCFBs */
         }

         this->wErrorID = MC_BufferActivateNext(pAxis, this, &fbActive);
         if (this->wErrorID == MC_NO_ERROR)
         {
            if(fbActive)
            {
               /* this FB is now active */
               pAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISC_MOTION;
               this->ChangeState(FBS_ACTIVE);
            }
         }
         else
         {
            this->ChangeState(FBS_ERROR);
         }

         break;
      }
   case MC_PLCOPEN_STATE_STOPPING:
      {
         /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
          * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
          */
         this->wErrorID = MC_ERR_BUSY_STOPPING;
         this->ChangeState(FBS_ERROR);
         break;
      }
   default:
      {
         this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
         this->ChangeState(FBS_ERROR);
      }
   }
}

void MC_MOVE_ABSOLUTE_T::StateTrans_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;
   MC_T_POS_CALC   lTargetPosition = (MC_T_POS_CALC)(this->fPosition * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
   MC_T_POS_CALC   lDistance = 0;

   if (lTargetPosition > pAxis->lCommandedPosition)
   {
       /* are we moving into the wrong direction ? */
       if ((pAxis->dwDirection != MC_DIR_POSITIVE) && (pMove->lCommandVel != 0))
       {
           MC_T_VELACC lNextVel;
           lNextVel = MC_TrajFollowingSetPoint(&pAxis->oMove);
           if(lNextVel != 0)
           {
               this->wErrorID = MC_ERR_DIR_CHANGED;
               this->ChangeState(FBS_ERROR);
               return;
           }
       }

       lDistance = lTargetPosition - pAxis->lCommandedPosition;
       pAxis->dwDirection = MC_DIR_POSITIVE;
   }
   else
   {
       /* are we moving into the wrong direction ? */
       if ((pAxis->dwDirection != MC_DIR_NEGATIVE) && (pMove->lCommandVel != 0))
       {
           MC_T_VELACC lNextVel;
           lNextVel = MC_TrajFollowingSetPoint(&pAxis->oMove);
           if(lNextVel != 0)
           {
               this->wErrorID = MC_ERR_DIR_CHANGED;
               this->ChangeState(FBS_ERROR);
               return;
           }
       }

       lDistance = pAxis->lCommandedPosition - lTargetPosition;
       pAxis->dwDirection = MC_DIR_NEGATIVE;
   }

   pMove->bContUpdate = this->bContUpdate;
   pMove->lTargetPosition = lTargetPosition;

   /* SW limits enabled ? */
   if (pAxis->bSWLimitNegativeEna || pAxis->bSWLimitPositiveEna)
   {
       this->wErrorID = MC_CheckSoftwareLimits(pAxis, lDistance, EC_NULL);
       if (this->wErrorID != MC_NO_ERROR)
       {
           this->ChangeState(FBS_ERROR);
           return;
       }
   }

   pMove->lTotalDist = lDistance;
   pMove->lNextMoveCorrection = 0;  /* no remainder from previous move, because we want to move to an absolute pos */

   /* calculate movement and setup parameters */
   if (pAxis->eSetValSource == SETVAL_TRAJGEN_ECM)
   {
       if(this->fJerk != 0)
       {
          this->wErrorID = MC_TrajSetupJerkMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration, this->fJerk);
       }
       else
       {
          this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);
       }
   }
#ifdef EC_MOTION_SUPPORT_PP_MODE
   else if (pAxis->eSetValSource == SETVAL_TRAJGEN_DRV)
   {
        /* was machen? */
        this->wErrorID = MC_SetupTrajInDriveMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, this->fAcceleration, this->fDeceleration);

        /* Request new profile */
        pMove->eProfileState = eProfileStatus_NewSetPoint;
   }
#endif

   if(this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }
}

void MC_MOVE_ABSOLUTE_T::DoState_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;

   if(pAxis->ePLCOpenState != MC_PLCOPEN_STATE_DISC_MOTION)
   {
      this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
      this->ChangeState(FBS_ERROR);
      return;
   }

   // if vel or acc or dec or distance changed, we have to recalc
   if(pMove->bContUpdate)
   {
      MC_T_POS_CALC   lDistance = pMove->lTotalDist;
      MC_T_POS_CALC lTargetPosition = (MC_T_POS_CALC)(this->fPosition * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
      if(pMove->lTargetPosition != lTargetPosition)   /* target position changed ? */
      {
         if(lTargetPosition > pAxis->lCommandedPosition)
         {
            if(pAxis->dwDirection != MC_DIR_POSITIVE)
            {
               this->wErrorID = MC_ERR_CONTUPDATE_DIR_CHANGED;
               this->ChangeState(FBS_ERROR);
               return;
            }

            lDistance = lTargetPosition - pMove->lTargetPosition + pMove->lTotalDist;
            pMove->lTargetPosition = lTargetPosition;
         }
         else
         {
            if(pAxis->dwDirection != MC_DIR_NEGATIVE)
            {
               this->wErrorID = MC_ERR_CONTUPDATE_DIR_CHANGED;
               this->ChangeState(FBS_ERROR);
               return;
            }

            lDistance = pMove->lTargetPosition - lTargetPosition + pMove->lTotalDist;
            pMove->lTargetPosition = lTargetPosition;
         }
      }

      /* calculate movement and setup parameters */
      if(INPUT_PARA_CHANGED(pMove, lDistance, this->fVelocity, this->fAcceleration, this->fDeceleration))
      {
         /* SW limits enabled ? */
         if(pAxis->bSWLimitNegativeEna || pAxis->bSWLimitPositiveEna)
         {
            this->wErrorID = MC_CheckSoftwareLimits(pAxis, lDistance, EC_NULL);
            if (this->wErrorID != MC_NO_ERROR)
            {
               this->ChangeState(FBS_ERROR);
               return;
            }
         }

         if(this->fJerk != 0)
         {
            this->wErrorID = MC_TrajSetupJerkMove(pAxis, pMove, MC_FALSE, MC_FALSE, lDistance,
               this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration, this->fJerk);
         }
         else
         {
            this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_FALSE, MC_FALSE, lDistance,
               this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);
         }

         if (this->wErrorID != MC_NO_ERROR)
         {
            this->ChangeState(FBS_ERROR);
            return;
         }
      }
   }

   MC_T_BOOL bDoneLocal = MC_FALSE;
#ifdef EC_MOTION_SUPPORT_PP_MODE
   if(pAxis->eOperationMode == OPMODE_PP)
   {
        this->wErrorID = MC_CheckTargetPosReachedTrajInDrive(pAxis, &bDoneLocal);
   }
   else
#endif
   {
        this->wErrorID = MC_CheckTargetPosReached(pAxis, &bDoneLocal);
   }
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   if (bDoneLocal)
   {
      this->ChangeState(FBS_DONE);
      pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
   }
}

MC_T_BOOL MC_MOVE_ABSOLUTE_T::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ACTIVE)
         {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
         }

         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_ACTIVE:
      {
         DoState_Active();
         if (this->fbState == FBS_DONE)
         {
            this->bDone = MC_TRUE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            continueSm = MC_TRUE;
         }
         else if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
   case FBS_ERROR:
   case FBS_DONE:
      {
         /* 2.4.1 General rules - Output status
          * Done, Error, ErrorID and CommandAborted are reset with the falling edge
          * of Execute.
          * It must be guaranted that the coresponding outputs are set for at least
          * one cycle...
          */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bDone = MC_FALSE;
            this->bCommandAborted = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         break;
      }
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->bDone, this->bError, this->bCommandAborted));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bDone, this->bError, this->bCommandAborted));

   return continueSm;
}

MC_T_WORD MC_MOVE_ABSOLUTE_T::BlendingRecalcMove(MC_T_BUFFERENTRY* pPrevFb, MC_T_REAL fDistance)
{
    MC_T_MOVEMENT*       pMove = &pAxis->oMove;
    MC_T_MOVEMENT        myMove;
    MC_T_VELACC          lEndVel;
    EC_T_WORD            wRes = MC_NO_ERROR;

    /* is the currently active FB effected ? */
    if((pAxis->pWorkFb->pFB == pPrevFb->pFB))
    {
        lEndVel = (MC_T_VELACC)(pPrevFb->fEndVel * pAxis->fVelToInc);

        /* reduce EndVel of previous move id required to stop with this move */
        if((dwBufferMode == MC_BLENDING_PREVIOUS_STOP) ||(dwBufferMode == MC_BLENDING_NEXT_STOP) || (dwBufferMode == MC_BLENDING_HIGH_STOP ))
        {
            wRes = MC_CalcMoveProfileBuffered(pAxis, fDistance, fVelocity, fAcceleration, fDeceleration, fJerk,
                                              dwBufferMode, 0, pPrevFb->fEndVel, 0, &myMove);

            if(wRes == MC_ERR_CALC_END_VEL_TO_LOW)
            {
                EC_T_INT        nCalcLoops = 0;
                MC_T_REAL       fEndVel;

                fEndVel = sqrt(2 * fDistance * fDeceleration);
                do
                {
                    nCalcLoops++;
                    pPrevFb->fEndVel  = fEndVel * 0.97;     /* reduce by 3% to consider jerk etc. */

                    wRes = MC_CalcMoveProfileBuffered(pAxis, fDistance, fVelocity, fAcceleration, fDeceleration, fJerk,
                                                      dwBufferMode, 0, pPrevFb->fEndVel, 0, &myMove);
                }
                while( (wRes == MC_ERR_CALC_END_VEL_TO_LOW) && (nCalcLoops < 10));

#ifdef DEBUG
                if(nCalcLoops > 10)
                    EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR ERROR MC_MOVE_ABSOLUTE_T::BlendingRecalc\n"));
#endif

                lEndVel = EC_MIN(lEndVel, myMove.lConstantVel);

                if(pMove->eTrajectState == eTrajStatus_Dec)
                    lEndVel = EC_MIN(lEndVel, pMove->lCommandVel);

                if(pMove->lRemainDist < lEndVel)
                    return MC_NO_ERROR;
            }
        }

        pAxis->pWorkFb->fEndVel = pPrevFb->fEndVel = (MC_T_REAL)lEndVel/pAxis->fVelToInc;

        if(fJerk != 0)
        {
            return MC_TrajSetupJerkMove(pAxis, pMove, MC_FALSE, MC_FALSE, pMove->lTotalDist,
                pMove->fOrgMaxVelocity, pAxis->pWorkFb->fEndVel, pMove->fOrgAcceleration, pMove->fOrgDeceleration, pMove->fOrgJerk);
        }
        else
        {
            return MC_TrajSetupTrapezeMove(pAxis, pMove, MC_FALSE, MC_FALSE, pMove->lTotalDist,
                pMove->fOrgMaxVelocity, pAxis->pWorkFb->fEndVel, pMove->fOrgAcceleration, pMove->fOrgDeceleration);
        }
    }

    return MC_NO_ERROR;
}

void MC_MOVE_ABSOLUTE_T::OnCycle()
{
   while (StateMachine());
}


/********************************************************************************/
/** \brief MC_MoveRelative Move relative (3.6)

A relative motion is commanded with the MC_MoveRelative FB, in addition to the current
position. This is regardless of whether the axis is in the Discrete Motion or Continuous
Motion state. The motion is always executed jerk-limited.
This value is valid for both acceleration and deceleration.

*/
void MC_MOVE_RELATIVE_T::StateTrans_Execute()
{
   MC_T_BUFFERENTRY*    pPrevFb;

   if(pAxis == EC_NULL)
   {
      this->wErrorID = MC_ERR_INVALID_PARAMETER;
      this->ChangeState(FBS_ERROR);
      return;
   }

   /* check if move(input) parameters are valid */
   if(this->fDistance > pAxis->fMaxDistance)
   {
      this->wErrorID = MC_ERR_DIST_OUT_OF_RANGE;
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->wErrorID = CheckTrajParams(pAxis, this->fVelocity, this->fAcceleration, this->fDeceleration, this->fJerk);
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->wErrorID = HandleBlending(pAxis, this, this->dwBufferMode, this->fVelocity, 0, &pPrevFb);

   /* do we have to recalc the current movement ? */
   if ((this->wErrorID == MC_NO_ERROR) && (pPrevFb != EC_NULL))
   {
       this->wErrorID = BlendingRecalcMove(pPrevFb, fabs(fDistance));
   }
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->ChangeState(FBS_BUSY);
}

void MC_MOVE_RELATIVE_T::DoState_Busy()
{
   switch (pAxis->ePLCOpenState)
   {
   case MC_PLCOPEN_STATE_STAND_STILL:
   case MC_PLCOPEN_STATE_DISC_MOTION:
   case MC_PLCOPEN_STATE_CONT_MOTION:
      {
         MC_T_BOOL  fbActive = false;

         if(this->dwBufferMode == MC_ABORTING)
         {
             MC_AbortCommand(pAxis, MCFB_ID_MOVE_RELATIVE);     /* abort other MCFBs */
         }

         this->wErrorID = MC_BufferActivateNext(pAxis, this, &fbActive);
         if (this->wErrorID == MC_NO_ERROR)
         {
            if(fbActive)
            {
               /* this FB is now active */
               pAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISC_MOTION;
               this->ChangeState(FBS_ACTIVE);
            }
         }
         else
         {
            this->ChangeState(FBS_ERROR);
         }

         break;
      }
   case MC_PLCOPEN_STATE_STOPPING:
      {
         /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
          * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
          */
         this->wErrorID = MC_ERR_BUSY_STOPPING;
         this->ChangeState(FBS_ERROR);
         break;
      }
   default:
      {
         this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
         this->ChangeState(FBS_ERROR);
      }
   }
}

void MC_MOVE_RELATIVE_T::StateTrans_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;
   MC_T_POS_CALC   lDistance = 0;

   if(this->fDistance >= 0)
   {
      /* are we moving into the wrong direction ? */
      if((pAxis->dwDirection != MC_DIR_POSITIVE) && (pMove->lCommandVel != 0))
      {
         MC_T_VELACC lNextVel;
         lNextVel = MC_TrajFollowingSetPoint(&pAxis->oMove);
         if(lNextVel != 0)
         {
            this->wErrorID = MC_ERR_DIR_CHANGED;
            this->ChangeState(FBS_ERROR);
            return;
         }
      }

      lDistance = (MC_T_POS_CALC)(this->fDistance * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
      pAxis->dwDirection = MC_DIR_POSITIVE;
   }
   else
   {
      /* are we moving into the wrong direction ? */
      if((pAxis->dwDirection != MC_DIR_NEGATIVE) && (pMove->lCommandVel != 0))
      {
         MC_T_VELACC lNextVel;
         lNextVel = MC_TrajFollowingSetPoint(&pAxis->oMove);
         if(lNextVel != 0)
         {
            this->wErrorID = MC_ERR_DIR_CHANGED;
            this->ChangeState(FBS_ERROR);
            return;
         }
      }

      lDistance = (MC_T_POS_CALC)(-this->fDistance * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
      pAxis->dwDirection = MC_DIR_NEGATIVE;
   }

   pMove->bContUpdate = this->bContUpdate;

   /* SW limits enabled ? */
   if(pAxis->bSWLimitNegativeEna || pAxis->bSWLimitPositiveEna)
   {
      this->wErrorID = MC_CheckSoftwareLimits(pAxis, lDistance, EC_NULL);
      if (this->wErrorID != MC_NO_ERROR)
      {
         this->ChangeState(FBS_ERROR);
         return;
      }
   }

   pMove->lTotalDist = lDistance;

   /* calculate movement and setup parameters */
#ifdef MOTIONTEST
#define TRAJGEN_TEST
#ifdef TRAJGEN_TEST
   pAxis->pWorkFb->fEndVel = this->fEndVel;
#endif
#endif

   if (pAxis->eSetValSource == SETVAL_TRAJGEN_ECM)
   {
       if(this->fJerk != 0)
       {
          this->wErrorID = MC_TrajSetupJerkMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration, this->fJerk);
       }
       else
       {
          this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);
       }
   }
#ifdef EC_MOTION_SUPPORT_PP_MODE
   else if (pAxis->eSetValSource == SETVAL_TRAJGEN_DRV)
   {
        /* was machen? */
        this->wErrorID = MC_SetupTrajInDriveMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, this->fAcceleration, this->fDeceleration);

        /* Request new profile */
        pMove->eProfileState = eProfileStatus_NewSetPoint;
   }
#endif

   if(this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }
}

void MC_MOVE_RELATIVE_T::DoState_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;

   if(pAxis->ePLCOpenState != MC_PLCOPEN_STATE_DISC_MOTION)
   {
      this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
      this->ChangeState(FBS_ERROR);
      return;
   }

   // if vel or acc or dec or distance changed, we have to recalc
   if(pMove->bContUpdate)
   {
      MC_T_POS_CALC lDistance;

      if(this->fDistance >= 0)
      {
         if(pAxis->dwDirection != MC_DIR_POSITIVE)
         {
            this->wErrorID = MC_ERR_CONTUPDATE_DIR_CHANGED;  /* changing move direction is forbidden */
            this->ChangeState(FBS_ERROR);
            return;
         }

         lDistance = (MC_T_POS_CALC)(this->fDistance * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
      }
      else
      {
         if(pAxis->dwDirection != MC_DIR_NEGATIVE)
         {
            this->wErrorID = MC_ERR_CONTUPDATE_DIR_CHANGED; /* changing move direction is forbidden */
            this->ChangeState(FBS_ERROR);
            return;
         }

         lDistance = (MC_T_POS_CALC)(-this->fDistance * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
      }

      /* calculate movement and setup parameters */
      if(INPUT_PARA_CHANGED(pMove, lDistance, this->fVelocity, this->fAcceleration, this->fDeceleration))
      {
         /* SW limits enabled ? */
         if(pAxis->bSWLimitNegativeEna || pAxis->bSWLimitPositiveEna)
         {
            this->wErrorID = MC_CheckSoftwareLimits(pAxis, lDistance, EC_NULL);
            if (this->wErrorID != MC_NO_ERROR)
            {
               this->ChangeState(FBS_ERROR);
               return;
            }
         }

         if(this->fJerk != 0)
         {
            this->wErrorID = MC_TrajSetupJerkMove(pAxis, pMove, MC_FALSE, MC_FALSE, lDistance,
               this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration, this->fJerk);
         }
         else
         {
            this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_FALSE, MC_FALSE, lDistance,
               this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);
         }

         if (this->wErrorID != MC_NO_ERROR)
         {
            this->ChangeState(FBS_ERROR);
            return;
         }
      }
   }

   MC_T_BOOL bDoneLocal = MC_FALSE;
#ifdef EC_MOTION_SUPPORT_PP_MODE
   if(pAxis->eOperationMode == OPMODE_PP)
   {
        this->wErrorID = MC_CheckTargetPosReachedTrajInDrive(pAxis, &bDoneLocal);
   }
   else
#endif
   {
        this->wErrorID = MC_CheckTargetPosReached(pAxis, &bDoneLocal);
   }
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   if (bDoneLocal)
   {
      this->ChangeState(FBS_DONE);
      pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
   }
}

MC_T_BOOL MC_MOVE_RELATIVE_T::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ACTIVE)
         {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
         }

         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_ACTIVE:
      {
         DoState_Active();
         if (this->fbState == FBS_DONE)
         {
            this->bDone = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
         }
         else if (this->fbState == FBS_ERROR)
         {
            this->bError = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
   case FBS_ERROR:
   case FBS_DONE:
      {
         /* 2.4.1 General rules - Output status
          * Done, Error, ErrorID and CommandAborted are reset with the falling edge
          * of Execute.
          * It must be guaranted that the coresponding outputs are set for at least
          * one cycle...
          */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bDone = MC_FALSE;
            this->bCommandAborted = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         break;
      }
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->bDone, this->bError, this->bCommandAborted));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bDone, this->bError, this->bCommandAborted));

   return continueSm;
}

MC_T_WORD MC_MOVE_RELATIVE_T::BlendingRecalcMove(MC_T_BUFFERENTRY* pPrevFb, MC_T_REAL fDistanceParam)
{
    MC_T_MOVEMENT*       pMove = &pAxis->oMove;
    MC_T_MOVEMENT        myMove;
    MC_T_VELACC          lEndVel;
    EC_T_WORD            wRes = MC_NO_ERROR;

    /* is the currently active FB effected ? */
    if((pAxis->pWorkFb->pFB == pPrevFb->pFB))
    {
        lEndVel = (MC_T_VELACC)(pPrevFb->fEndVel * pAxis->fVelToInc);

        /* reduce EndVel of previous move id required to stop with this move */
        if((dwBufferMode == MC_BLENDING_PREVIOUS_STOP) ||(dwBufferMode == MC_BLENDING_NEXT_STOP) || (dwBufferMode == MC_BLENDING_HIGH_STOP ))
        {
            wRes = MC_CalcMoveProfileBuffered(pAxis, fDistanceParam, fVelocity, fAcceleration, fDeceleration, fJerk,
                                              dwBufferMode, 0, pPrevFb->fEndVel, 0, &myMove);

            if(wRes == MC_ERR_CALC_END_VEL_TO_LOW)
            {
                EC_T_INT        nCalcLoops = 0;
                MC_T_REAL       fEndVel;

                fEndVel = sqrt(2 * fDistanceParam * fDeceleration);
                do
                {
                    nCalcLoops++;
                    pPrevFb->fEndVel  = fEndVel * 0.97;     /* reduce by 3% to consider jerk etc. */

                    wRes = MC_CalcMoveProfileBuffered(pAxis, fDistanceParam, fVelocity, fAcceleration, fDeceleration, fJerk,
                                                      dwBufferMode, 0, pPrevFb->fEndVel, 0, &myMove);
                }
                while( (wRes == MC_ERR_CALC_END_VEL_TO_LOW) && (nCalcLoops < 10));

#ifdef DEBUG
                if(nCalcLoops > 10)
                    EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR ERROR MC_MOVE_RELATIVE_T::BlendingRecalc\n"));
#endif

                lEndVel = EC_MIN(lEndVel, myMove.lConstantVel);

                if(pMove->eTrajectState == eTrajStatus_Dec)
                    lEndVel = EC_MIN(lEndVel, pMove->lCommandVel);

                if(pMove->lRemainDist < lEndVel)
                    return MC_NO_ERROR;
            }
        }

        pAxis->pWorkFb->fEndVel = pPrevFb->fEndVel = (MC_T_REAL)lEndVel/pAxis->fVelToInc;

        if(fJerk != 0)
        {
            return MC_TrajSetupJerkMove(pAxis, pMove, MC_FALSE, MC_FALSE, pMove->lTotalDist,
                pMove->fOrgMaxVelocity, pAxis->pWorkFb->fEndVel, pMove->fOrgAcceleration, pMove->fOrgDeceleration, pMove->fOrgJerk);
        }
        else
        {
            return MC_TrajSetupTrapezeMove(pAxis, pMove, MC_FALSE, MC_FALSE, pMove->lTotalDist,
                pMove->fOrgMaxVelocity, pAxis->pWorkFb->fEndVel, pMove->fOrgAcceleration, pMove->fOrgDeceleration);
        }
    }

    return MC_NO_ERROR;
}

void MC_MOVE_RELATIVE_T::OnCycle()
{
   while (StateMachine());
}


/********************************************************************************/
/** \brief MC_MoveVelocity Move velocity (3.10)

With the MC_MoveVelocity FB, you command an endless motion at the specified speed.
The motion is always executed jerk-limited with the constant set input ‘jerk’. This value is
valid for both acceleration and deceleration.
To stop the motion, the FB must be interrupted by another FB that sends a new command to the axis.
The bInVelocity output is reset when a falling edge is detected at the execute input or
when the effect of the FB is ended due to commanding of another FB.

*/
void MC_MOVE_VELOCITY_T::StateTrans_Execute()
{
   if(pAxis == EC_NULL)
   {
      this->wErrorID = MC_ERR_INVALID_PARAMETER;
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->wErrorID = CheckTrajParams(pAxis, this->fVelocity, this->fAcceleration, this->fDeceleration, this->fJerk);
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   switch (this->dwBufferMode)
   {
   case MC_BLENDING_LOW:
   case MC_BLENDING_PREVIOUS:
   case MC_BLENDING_PREVIOUS_STOP:
   case MC_BLENDING_NEXT:
   case MC_BLENDING_NEXT_STOP:
   case MC_BLENDING_HIGH:
   case MC_BLENDING_HIGH_STOP:
      {
         OsDbgAssert(MC_FALSE);
         EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "%s: Error Buffermode%d not supported\n", GetName(), this->dwBufferMode));
         this->wErrorID = MC_ERR_BUFFERMODE_NOT_SUPP;

         this->ChangeState(FBS_ERROR);
         break;
      }
   default:
      {
         MC_T_BUFFERENTRY*   pPrevFb = EC_NULL;

         /* put FB into buffer */
         this->wErrorID = MC_BufferPut(pAxis, this, 0, &pPrevFb);
         if (this->wErrorID != MC_NO_ERROR)
         {
            this->ChangeState(FBS_ERROR);
            return;
         }

         this->ChangeState(FBS_BUSY);
      }
   }
}

void MC_MOVE_VELOCITY_T::DoState_Busy()
{
   switch (pAxis->ePLCOpenState)
   {
   case MC_PLCOPEN_STATE_STAND_STILL:
   case MC_PLCOPEN_STATE_DISC_MOTION:
   case MC_PLCOPEN_STATE_CONT_MOTION:
      {
         MC_T_BOOL  fbActive = false;

         if(this->dwBufferMode == MC_ABORTING)
         {
             MC_AbortCommand(pAxis, MCFB_ID_MOVE_VELOCITY);     /* abort other MCFBs */
         }
         /* In buffered mode the velocity would reach bInVelocity before actuating the next FB */
         else if (    this->dwBufferMode == MC_BUFFERED
                   && pAxis->pWorkFb->pFB->wFbID == MCFB_ID_MOVE_VELOCITY)
         {
            MC_MOVE_VELOCITY_T*     pPrevMcfb = (MC_MOVE_VELOCITY_T*)pAxis->pWorkFb->pFB;

            if(pPrevMcfb->bInVelocity)
            {
               MC_AbortCommand(pAxis, MCFB_ID_MOVE_VELOCITY);     /* abort other MCFBs */
            }
         }

         /* remove FB from buffer */
         this->wErrorID = MC_BufferActivateNext(pAxis, this, &fbActive);
         if (this->wErrorID == MC_NO_ERROR)
         {
            if(fbActive)
            {
               /* this FB is now active */
               pAxis->ePLCOpenState = MC_PLCOPEN_STATE_CONT_MOTION;   /* velocity in progress */
               this->ChangeState(FBS_ACTIVE);
            }
         }
         else
         {
            this->ChangeState(FBS_ERROR);
         }
         break;
      }
   case MC_PLCOPEN_STATE_STOPPING:
      {
         /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
          * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
          */
         this->wErrorID = MC_ERR_BUSY_STOPPING;
         this->ChangeState(FBS_ERROR);
         break;
      }
   default:
      {
         this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
         this->ChangeState(FBS_ERROR);
      }
   }
}

void MC_MOVE_VELOCITY_T::StateTrans_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;

   if(this->fVelocity <= 0)
   {
      this->wErrorID = MC_ERR_VEL_OUT_OF_RANGE;
      this->ChangeState(FBS_ERROR);
      return;
   }

   switch (this->dwDirection)
   {
   case MC_DIR_POSITIVE:
      pAxis->dwDirection = MC_DIR_POSITIVE; break;
   case MC_DIR_NEGATIVE:
      pAxis->dwDirection = MC_DIR_NEGATIVE; break;
   case MC_DIR_CURRENT:
      /* NOOP */; break;
   default:
      this->wErrorID = MC_ERR_INVALID_DIRECTION;
      this->ChangeState(FBS_ERROR);
      return;
   }

   MC_T_POS_CALC lDistance = 0;
   MC_T_BOOL     bEndless = MC_TRUE;

   pMove->bContUpdate     =  this->bContUpdate;
   pAxis->pWorkFb->fEndVel = this->fVelocity;

   /* SW limits enabled ? */
   if(pAxis->bSWLimitPositiveEna && (pAxis->dwDirection == MC_DIR_POSITIVE))
   {
      lDistance = EC_MAX(0, pAxis->lSWLimitPositivePos - pAxis->lCommandedPosition - 1);
      pAxis->pWorkFb->fEndVel = 0;
      bEndless = MC_FALSE;
   }

   if(pAxis->bSWLimitNegativeEna && (pAxis->dwDirection == MC_DIR_NEGATIVE))
   {
      lDistance = EC_MAX(0, pAxis->lCommandedPosition - pAxis->lSWLimitNegativePos - 1);
      pAxis->pWorkFb->fEndVel = 0;
      bEndless = MC_FALSE;
   }

   /* calculate movement and setup parameters */
   if (pAxis->eSetValSource == SETVAL_TRAJGEN_ECM)
   {
       if(this->fJerk != 0)
       {
          this->wErrorID = MC_TrajSetupJerkMove(pAxis, pMove, MC_TRUE, bEndless, lDistance,
             this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration, this->fJerk);
       }
       else
       {
          this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_TRUE, bEndless, lDistance,
             this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);
       }
   }
#ifdef EC_MOTION_SUPPORT_PP_MODE
   else if (pAxis->eSetValSource == SETVAL_TRAJGEN_DRV)
   {
       /* was machen? */
       this->wErrorID = MC_SetupTrajInDriveMove(pAxis, pMove, MC_TRUE, bEndless, lDistance,
           this->fVelocity, this->fAcceleration, this->fDeceleration);
       /* Request new profile */
       pMove->eProfileState = eProfileStatus_NewSetPoint;
   }
#endif

   if(this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   if(pMove->bEndless)
   {
      pAxis->oMove.lRemainDist = pMove->lSegDist1to3;
   }
}

void MC_MOVE_VELOCITY_T::DoState_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;

   if(pAxis->ePLCOpenState != MC_PLCOPEN_STATE_CONT_MOTION)
   {
      this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
      this->ChangeState(FBS_ERROR);
      return;
   }

   // if vel or acc or dec changed, we have to recalc
   if(pMove->bContUpdate)
   {
      MC_T_POS_CALC lDistance = pMove->lTotalDist;

      /* calculate movement and setup parameters */
      if(INPUT_PARA_CHANGED(pMove, lDistance, this->fVelocity, this->fAcceleration, this->fDeceleration))
      {
         pAxis->pWorkFb->fEndVel = this->fVelocity;

         if(this->fJerk != 0)
         {
            this->wErrorID = MC_TrajSetupJerkMove(pAxis, pMove, MC_FALSE, pMove->bEndless, lDistance,
               this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration, this->fJerk);
         }
         else
         {
            this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_FALSE, pMove->bEndless, lDistance,
               this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);
         }

         if(this->wErrorID != MC_NO_ERROR)
         {
            this->ChangeState(FBS_ERROR);
            return;
         }

         if(pMove->bEndless)
         {
            pAxis->oMove.lRemainDist = pMove->lSegDist1to3;
         }
      }
   }

   MC_T_BOOL bDoneLocal = MC_FALSE;

   /* in case of moving to a software limit position we have to check for done */
   if(!pAxis->oMove.bEndless)
   {
      this->wErrorID = MC_CheckTargetPosReached(pAxis, &bDoneLocal);
      if(this->wErrorID != MC_NO_ERROR)
      {
         this->ChangeState(FBS_ERROR);
         return;
      }
   }

   if(bDoneLocal)
   {
      pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
      this->wErrorID = (EC_T_WORD) ((this->dwDirection == MC_DIR_POSITIVE) ? MC_ERR_SW_LIMIT_POS : MC_ERR_SW_LIMIT_NEG);
      this->ChangeState(FBS_ERROR);
   }

   MC_CheckTargetVelReached(pAxis, &this->bInVelocity);
}

MC_T_BOOL MC_MOVE_VELOCITY_T::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ACTIVE)
         {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
         }

         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_ACTIVE:
      {
         DoState_Active(); // this->bInVelocity is set if the target velocity has been reached.
         if (this->fbState == FBS_ERROR)
         {
            this->bInVelocity = MC_FALSE; // Done, InXXX, ... so these signals are logically exclusive to Error.
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
   case FBS_ERROR:
      {
         /* 2.4.1 General rules - Output status
          * Done, Error, ErrorID and CommandAborted are reset with the falling edge
          * of Execute.
          * It must be guaranted that the coresponding outputs are set for at least
          * one cycle...
          */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bCommandAborted = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         break;
      }
   default:
      break;
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->bError, this->bCommandAborted));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bError, this->bCommandAborted));

   return continueSm;
}

void MC_MOVE_VELOCITY_T::OnCycle()
{
   while (StateMachine());
}


/********************************************************************************/
/** \brief Move to an absolute position ending with the specified velocity (3.11)

The motion is always executed jerk-limited.
This value is valid for both acceleration and deceleration.

*/
void MC_MOVE_CONT_ABSOLUTE_T::StateTrans_Execute()
{
   MC_T_BUFFERENTRY*    pPrevFb;

   if(pAxis == EC_NULL)
   {
      this->wErrorID = MC_ERR_INVALID_PARAMETER;
      this->ChangeState(FBS_ERROR);
      return;
   }

   /* check if move(input) parameters are valid */
   this->wErrorID = CheckTrajParams(pAxis, this->fVelocity, this->fAcceleration, this->fDeceleration, this->fJerk);
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->wErrorID = HandleBlending(pAxis, this, this->dwBufferMode, this->fVelocity, this->fEndVelocity, &pPrevFb);

   /* do we have to recalc the current movement ? */
   if ((this->wErrorID == MC_NO_ERROR) && (pPrevFb != EC_NULL))
   {
       MC_T_REAL fDistanceLocal = ((MC_T_REAL)pAxis->oMove.lTargetPosition/pAxis->dwCalcIncPerMM) - this->fPosition;

       fDistanceLocal = fabs(fDistanceLocal);
       // todo this->wErrorID = BlendingRecalcMove(pPrevFb, fDistanceLocal);
   }

   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->ChangeState(FBS_BUSY);
}

void MC_MOVE_CONT_ABSOLUTE_T::DoState_Busy()
{
   switch (pAxis->ePLCOpenState)
   {
   case MC_PLCOPEN_STATE_STAND_STILL:
   case MC_PLCOPEN_STATE_DISC_MOTION:
   case MC_PLCOPEN_STATE_CONT_MOTION:
      {
         MC_T_BOOL fbActive = false;
         if(this->dwBufferMode == MC_ABORTING)
         {
            MC_AbortCommand(pAxis, MCFB_ID_MOVE_CONTINUOUS_ABSOLUTE);     /* abort other MCFBs */
         }

         this->wErrorID = MC_BufferActivateNext(pAxis, this, &fbActive);
         if (this->wErrorID == MC_NO_ERROR)
         {
            if(fbActive)
            {
               /* this FB is now active */
               pAxis->ePLCOpenState = MC_PLCOPEN_STATE_CONT_MOTION;
               this->ChangeState(FBS_ACTIVE);
            }
         }
         else
         {
            this->ChangeState(FBS_ERROR);
         }
         break;
      }
   case MC_PLCOPEN_STATE_STOPPING:
      {
         /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
          * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
          */
         this->wErrorID = MC_ERR_BUSY_STOPPING;
         this->ChangeState(FBS_ERROR);
         break;
      }
   default:
      {
         this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
         this->ChangeState(FBS_ERROR);
      }
   }
}

void MC_MOVE_CONT_ABSOLUTE_T::StateTrans_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;
   MC_T_POS_CALC   lTargetPosition = (MC_T_POS_CALC)(this->fPosition * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
   MC_T_POS_CALC   lDistance = 0;

   if (lTargetPosition > pAxis->lCommandedPosition)
   {
       /* are we moving into the wrong direction ? */
       if ((pAxis->dwDirection != MC_DIR_POSITIVE) && (pMove->lCommandVel != 0))
       {
           this->wErrorID = MC_ERR_DIR_CHANGED;
           this->ChangeState(FBS_ERROR);
           return;
       }

       lDistance = lTargetPosition - pAxis->lCommandedPosition;
       pAxis->dwDirection = MC_DIR_POSITIVE;
   }
   else
   {
       /* are we moving into the wrong direction ? */
       if ((pAxis->dwDirection != MC_DIR_NEGATIVE) && (pMove->lCommandVel != 0))
       {
           this->wErrorID = MC_ERR_DIR_CHANGED;
           this->ChangeState(FBS_ERROR);
           return;
       }

       lDistance = pAxis->lCommandedPosition - lTargetPosition;
       pAxis->dwDirection = MC_DIR_NEGATIVE;
   }

   pMove->bContUpdate = this->bContUpdate;
   pMove->lTargetPosition = lTargetPosition;

   /* SW limits enabled ? */
   if (pAxis->bSWLimitNegativeEna || pAxis->bSWLimitPositiveEna)
   {
       this->wErrorID = MC_CheckSoftwareLimits(pAxis, lDistance, EC_NULL);
       if (this->wErrorID != MC_NO_ERROR)
       {
           this->ChangeState(FBS_ERROR);
           return;
       }
   }

   pMove->lTotalDist = lDistance;

   /* calculate movement and setup parameters */
   if(pAxis->eSetValSource == SETVAL_TRAJGEN_ECM)
   {
       if(this->fJerk != 0)
       {
          this->wErrorID = MC_TrajSetupJerkMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration, this->fJerk);
       }
       else
       {
          this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);
       }
   }
#ifdef EC_MOTION_SUPPORT_PP_MODE
   else if(pAxis->eSetValSource == SETVAL_TRAJGEN_DRV)
   {
        /* was machen? */
        this->wErrorID = MC_SetupTrajInDriveMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, this->fAcceleration, this->fDeceleration);

        /* Request new profile */
        pMove->eProfileState = eProfileStatus_NewSetPoint;
   }
#endif

   if(this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }
}

void MC_MOVE_CONT_ABSOLUTE_T::DoState_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;

   if(pAxis->ePLCOpenState != MC_PLCOPEN_STATE_CONT_MOTION)
   {
      this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
      this->ChangeState(FBS_ERROR);
      return;
   }

   // if vel or acc or dec or distance changed, we have to recalc
   if(pMove->bContUpdate)
   {
      MC_T_POS_CALC   lDistance = pMove->lTotalDist;
      MC_T_POS_CALC lTargetPosition = (MC_T_POS_CALC)(this->fPosition * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
      if(pMove->lTargetPosition != lTargetPosition)   /* target position changed ? */
      {
         if(lTargetPosition > pAxis->lCommandedPosition)
         {
            if(pAxis->dwDirection != MC_DIR_POSITIVE)
            {
               this->wErrorID = MC_ERR_CONTUPDATE_DIR_CHANGED;
               this->ChangeState(FBS_ERROR);
               return;
            }

            lDistance = lTargetPosition - pMove->lTargetPosition + pMove->lTotalDist;
            pMove->lTargetPosition = lTargetPosition;
         }
         else
         {
            if(pAxis->dwDirection != MC_DIR_NEGATIVE)
            {
               this->wErrorID = MC_ERR_CONTUPDATE_DIR_CHANGED;
               this->ChangeState(FBS_ERROR);
               return;
            }

            lDistance = pMove->lTargetPosition - lTargetPosition + pMove->lTotalDist;
            pMove->lTargetPosition = lTargetPosition;
         }
      }

      /* calculate movement and setup parameters */
      if(INPUT_PARA_CHANGED(pMove, lDistance, this->fVelocity, this->fAcceleration, this->fDeceleration))
      {
         /* SW limits enabled ? */
         if(pAxis->bSWLimitNegativeEna || pAxis->bSWLimitPositiveEna)
         {
            this->wErrorID = MC_CheckSoftwareLimits(pAxis, lDistance, EC_NULL);
            if (this->wErrorID != MC_NO_ERROR)
            {
               this->ChangeState(FBS_ERROR);
               return;
            }
         }

         if(this->fJerk != 0)
         {
            this->wErrorID = MC_TrajSetupJerkMove(pAxis, pMove, MC_FALSE, MC_FALSE, lDistance,
               this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration, this->fJerk);
         }
         else
         {
            this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_FALSE, MC_FALSE, lDistance,
               this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);
         }

         if (this->wErrorID != MC_NO_ERROR)
         {
            this->ChangeState(FBS_ERROR);
            return;
         }
      }
   }

   MC_T_BOOL bDoneLocal = MC_FALSE;
#ifdef EC_MOTION_SUPPORT_PP_MODE
   if(pAxis->eOperationMode == OPMODE_PP)
   {
        this->wErrorID = MC_CheckTargetPosReachedTrajInDrive(pAxis, &bDoneLocal);
   }
   else
#endif
   {
        this->wErrorID = MC_CheckTargetPosReached(pAxis, &bDoneLocal);
   }
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   if (bDoneLocal)
   {
       // check if end velocity is commanded
       if(pAxis->oMove.lCommandVel == pAxis->oMove.lEndVel)
       {
           this->bInEndVelocity = MC_TRUE;
       }
       else
       {
           this->bInEndVelocity = MC_FALSE;
       }
       // stay in this state
   }
}

MC_T_BOOL MC_MOVE_CONT_ABSOLUTE_T::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_DONE: break;
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ACTIVE)
         {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
         }

         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_ACTIVE:
      {
         DoState_Active();
         if (this->fbState == FBS_ERROR)
         {
            this->bInEndVelocity = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
   case FBS_ERROR:
      {
         /* 2.4.1 General rules - Output status
          * Done, Error, ErrorID and CommandAborted are reset with the falling edge
          * of Execute.
          * It must be guaranted that the coresponding outputs are set for at least
          * one cycle...
          */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bCommandAborted = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         break;
      }
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->bError, this->bCommandAborted));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bInEndVelocity, this->bError, this->bCommandAborted));

   return continueSm;
}

void MC_MOVE_CONT_ABSOLUTE_T::OnCycle()
{
   while (StateMachine());
}


/********************************************************************************/
/** \brief MC_MoveContinuousRelative Move relative (3.12)

This Function Block commands a controlled motion of a specified relative distance
ending with the specified velocity.

*/
void MC_MOVE_CONT_RELATIVE_T::StateTrans_Execute()
{
   MC_T_BUFFERENTRY*    pPrevFb;

   if(pAxis == EC_NULL)
   {
      this->wErrorID = MC_ERR_INVALID_PARAMETER;
      this->ChangeState(FBS_ERROR);
      return;
   }

   /* check if move(input) parameters are valid */
   if(this->fDistance > pAxis->fMaxDistance)
   {
      this->wErrorID = MC_ERR_DIST_OUT_OF_RANGE;
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->wErrorID = CheckTrajParams(pAxis, this->fVelocity, this->fAcceleration, this->fDeceleration, this->fJerk);
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->wErrorID = HandleBlending(pAxis, this, this->dwBufferMode, this->fVelocity, this->fEndVelocity, &pPrevFb);

   /* do we have to recalc the current movement ? */
   if ((this->wErrorID == MC_NO_ERROR) && (pPrevFb != EC_NULL))
   {
       // todo this->wErrorID = BlendingRecalcMove(pPrevFb, fabs(fDistanceLocal));
   }

   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->ChangeState(FBS_BUSY);
}

void MC_MOVE_CONT_RELATIVE_T::DoState_Busy()
{
   switch (pAxis->ePLCOpenState)
   {
   case MC_PLCOPEN_STATE_STAND_STILL:
   case MC_PLCOPEN_STATE_DISC_MOTION:
   case MC_PLCOPEN_STATE_CONT_MOTION:
      {
         MC_T_BOOL  fbActive = false;

         if(this->dwBufferMode == MC_ABORTING)
         {
             MC_AbortCommand(pAxis, MCFB_ID_MOVE_CONTINUOUS_RELATIVE);     /* abort other MCFBs */
         }

         this->wErrorID = MC_BufferActivateNext(pAxis, this, &fbActive);
         if (this->wErrorID == MC_NO_ERROR)
         {
            if(fbActive)
            {
               /* this FB is now active */
               pAxis->ePLCOpenState = MC_PLCOPEN_STATE_CONT_MOTION;
               this->ChangeState(FBS_ACTIVE);
            }
         }
         else
         {
            this->ChangeState(FBS_ERROR);
         }
         break;
      }
   case MC_PLCOPEN_STATE_STOPPING:
      {
         /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
          * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
          */
         this->wErrorID = MC_ERR_BUSY_STOPPING;
         this->ChangeState(FBS_ERROR);
         break;
      }
   default:
      {
         this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
         this->ChangeState(FBS_ERROR);
      }
   }
}

void MC_MOVE_CONT_RELATIVE_T::StateTrans_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;
   MC_T_POS_CALC   lDistance = 0;

   if(this->fDistance >= 0)
   {
      /* are we moving into the wrong direction ? */
      if((pAxis->dwDirection != MC_DIR_POSITIVE) && (pMove->lCommandVel != 0))
      {
         this->wErrorID = MC_ERR_DIR_CHANGED;
         this->ChangeState(FBS_ERROR);
         return;
      }

      lDistance = (MC_T_POS_CALC)(this->fDistance * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
      pAxis->dwDirection = MC_DIR_POSITIVE;
   }
   else
   {
      /* are we moving into the wrong direction ? */
      if((pAxis->dwDirection != MC_DIR_NEGATIVE) && (pMove->lCommandVel != 0))
      {
         this->wErrorID = MC_ERR_DIR_CHANGED;
         this->ChangeState(FBS_ERROR);
         return;
      }

      lDistance = (MC_T_POS_CALC)(-this->fDistance * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
      pAxis->dwDirection = MC_DIR_NEGATIVE;
   }

   pMove->bContUpdate = this->bContUpdate;

   /* SW limits enabled ? */
   if(pAxis->bSWLimitNegativeEna || pAxis->bSWLimitPositiveEna)
   {
      this->wErrorID = MC_CheckSoftwareLimits(pAxis, lDistance, EC_NULL);
      if (this->wErrorID != MC_NO_ERROR)
      {
         this->ChangeState(FBS_ERROR);
         return;
      }
   }

   pMove->lTotalDist = lDistance;

   /* calculate movement and setup parameters */
   if(pAxis->eSetValSource == SETVAL_TRAJGEN_ECM)
   {
       if(this->fJerk != 0)
       {
          this->wErrorID = MC_TrajSetupJerkMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration, this->fJerk);
       }
       else
       {
          this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);
       }
   }
#ifdef EC_MOTION_SUPPORT_PP_MODE
   else if (pAxis->eSetValSource == SETVAL_TRAJGEN_DRV)
   {
        /* was machen? */
        this->wErrorID = MC_SetupTrajInDriveMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
             this->fVelocity, this->fAcceleration, this->fDeceleration);

        /* Request new profile */
        pMove->eProfileState = eProfileStatus_NewSetPoint;
   }
#endif

   if(this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }
}

void MC_MOVE_CONT_RELATIVE_T::DoState_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;

   if(pAxis->ePLCOpenState != MC_PLCOPEN_STATE_CONT_MOTION)
   {
      this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
      this->ChangeState(FBS_ERROR);
      return;
   }

   // if vel or acc or dec or distance changed, we have to recalc
   if(pMove->bContUpdate)
   {
      MC_T_POS_CALC lDistance;

      if(this->fDistance >= 0)
      {
         if(pAxis->dwDirection != MC_DIR_POSITIVE)
         {
            this->wErrorID = MC_ERR_CONTUPDATE_DIR_CHANGED;  /* changing move direction is forbidden */
            this->ChangeState(FBS_ERROR);
            return;
         }

         lDistance = (MC_T_POS_CALC)(this->fDistance * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
      }
      else
      {
         if(pAxis->dwDirection != MC_DIR_NEGATIVE)
         {
            this->wErrorID = MC_ERR_CONTUPDATE_DIR_CHANGED; /* changing move direction is forbidden */
            this->ChangeState(FBS_ERROR);
            return;
         }

         lDistance = (MC_T_POS_CALC)(-this->fDistance * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
      }

      /* calculate movement and setup parameters */
      if(INPUT_PARA_CHANGED(pMove, lDistance, this->fVelocity, this->fAcceleration, this->fDeceleration))
      {
         /* SW limits enabled ? */
         if(pAxis->bSWLimitNegativeEna || pAxis->bSWLimitPositiveEna)
         {
            this->wErrorID = MC_CheckSoftwareLimits(pAxis, lDistance, EC_NULL);
            if (this->wErrorID != MC_NO_ERROR)
            {
               this->ChangeState(FBS_ERROR);
               return;
            }
         }

         if(this->fJerk != 0)
         {
            this->wErrorID = MC_TrajSetupJerkMove(pAxis, pMove, MC_FALSE, MC_FALSE, lDistance,
               this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration, this->fJerk);
         }
         else
         {
            this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_FALSE, MC_FALSE, lDistance,
               this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);
         }

         if (this->wErrorID != MC_NO_ERROR)
         {
            this->ChangeState(FBS_ERROR);
            return;
         }
      }
   }

   MC_T_BOOL bDoneLocal = MC_FALSE;
#ifdef EC_MOTION_SUPPORT_PP_MODE
   if(pAxis->eOperationMode == OPMODE_PP)
   {
        this->wErrorID = MC_CheckTargetPosReachedTrajInDrive(pAxis, &bDoneLocal);
   }
   else
#endif
   {
        this->wErrorID = MC_CheckTargetPosReached(pAxis, &bDoneLocal);
   }
   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   if (bDoneLocal)
   {
       // check if end velocity is commanded
       if(pAxis->oMove.lCommandVel == pAxis->oMove.lEndVel)
       {
           this->bInEndVelocity = MC_TRUE;
       }
       else
       {
           this->bInEndVelocity = MC_FALSE;
       }
       // stay in this state
   }
}

MC_T_BOOL MC_MOVE_CONT_RELATIVE_T::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_DONE: break;
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ACTIVE)
         {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
         }

         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_ACTIVE:
      {
         DoState_Active();
         if (this->fbState == FBS_ERROR)
         {
            this->bInEndVelocity = MC_FALSE;
            this->bError = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
   case FBS_ERROR:
      {
         /* 2.4.1 General rules - Output status
          * Done, Error, ErrorID and CommandAborted are reset with the falling edge
          * of Execute.
          * It must be guaranted that the coresponding outputs are set for at least
          * one cycle...
          */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bCommandAborted = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         break;
      }
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->bError, this->bCommandAborted));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bInEndVelocity, this->bError, this->bCommandAborted));

   return continueSm;
}

void MC_MOVE_CONT_RELATIVE_T::OnCycle()
{
   while (StateMachine());
}


/********************************************************************************/
/** \brief MC_PositionProfile  (3.14)

This Function Block commands a time-position locked motion profile

*/
#ifdef EC_MOTION_SUPPORT_MC_POSITION_PROFILE

MC_POSITION_PROFILE_T::MC_POSITION_PROFILE_T(MC_T_AXIS_REF *pAxis, MC_T_TP_REF *pTimePosition)
: // Init from constructor
MC_BUFFERED_FB_T(pAxis, MCFB_ID_POSITION_PROFILE),
// Init external OUT's
Done(static_cast<const MC_T_BOOL &>(bDone)),
Busy(static_cast<const MC_T_BOOL &>(bBusy)),
// Init variables
Execute(0),
ContinuousUpdate(0),
TimeScale(0.0),
PositionScale(0.0),
Offset(0.0),
TimePosition(pTimePosition),
bDone(0),
bBusy(0),
// Init internal IN's
bExecute(static_cast<const MC_T_BOOL &>(Execute)),
bContUpdate(static_cast<const MC_T_BOOL &>(ContinuousUpdate)),
fTimeScale(static_cast<const MC_T_REAL &>(TimeScale)),
fPositionScale(static_cast<const MC_T_REAL &>(PositionScale)),
fOffset(static_cast<const MC_T_REAL &>(Offset)),
// Init internal IN_OUT's
pTimePosition(TimePosition)

{
    // Create Spline
    this->pSpline = EC_NEW(MC_T_SPLINE);

    OsDbgAssert(this->pSpline != EC_NULL);
}

MC_POSITION_PROFILE_T::~MC_POSITION_PROFILE_T()
{
    // Delete Spline
    delete(this->pSpline);
}


void MC_POSITION_PROFILE_T::StateTrans_Execute()
{
   MC_T_DWORD      dwPairIndex;
   MC_T_TP*       pTp;

   /* check if TimePosition is reasonable */
   if(pTimePosition->dwNumberOfPairs == 0)
   {
      this->wErrorID = MC_ERR_TP_REF_PAIRS_ZERO;
      this->ChangeState(FBS_ERROR);
      return;
   }

   /* check control points */
   for(dwPairIndex = 0, pTp = pTimePosition->pTimePos; dwPairIndex < pTimePosition->dwNumberOfPairs; dwPairIndex++, pTp++)
   {
      if(pTp->fDeltaTime == 0.0)
      {
         this->wErrorID = MC_ERR_TP_REF_DELTATIME_ZERO;
         break;
      }
      if(pTp->fDeltaTime < 0)
      {
         this->wErrorID = MC_ERR_TP_REF_DELTATIME_NEG;
         break;
      }
   }

   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->wErrorID = HandleBlending(pAxis, this, this->dwBufferMode, pAxis->fMaxVel, pAxis->fMaxJerk);

       this->wErrorID = BlendingRecalcMove(pPrevFb, fabs(fDistanceLocal));

   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   this->ChangeState(FBS_BUSY);
}

void MC_POSITION_PROFILE_T::DoState_Busy()
{
   switch (pAxis->ePLCOpenState)
   {
   case MC_PLCOPEN_STATE_STAND_STILL:
   case MC_PLCOPEN_STATE_DISC_MOTION:
   case MC_PLCOPEN_STATE_CONT_MOTION:
      {
         MC_T_BOOL  fbActive = false;

         if(this->dwBufferMode == MC_ABORTING)
         {
             MC_AbortCommand(pAxis, MCFB_ID_POSITION_PROFILE);     /* abort other MCFBs */
         }

         this->wErrorID = MC_BufferActivateNext(pAxis, this, &fbActive);
         if (this->wErrorID == MC_NO_ERROR)
         {
            if(fbActive)
            {
               /* this FB is now active */
               pAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISC_MOTION;
               this->ChangeState(FBS_ACTIVE);
            }
         }
         else
         {
            this->ChangeState(FBS_ERROR);
         }

         break;
      }
   case MC_PLCOPEN_STATE_STOPPING:
      {
         /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
          * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
          */
         this->wErrorID = MC_ERR_BUSY_STOPPING;
         this->ChangeState(FBS_ERROR);
         break;
      }
   default:
      {
         this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
         this->ChangeState(FBS_ERROR);
      }
   }
}

void MC_POSITION_PROFILE_T::StateTrans_Active()
{
   MC_T_MOVEMENT*  pMove = &pAxis->oMove;
   MC_T_DWORD      dwPairIndex;
   MC_T_TP*       pTp;
   MC_T_REAL      fCommandedPos = 0.0;
   MC_T_BOOL      bDoTimeScale = MC_FALSE;
   MC_T_BOOL      bDoPositionScale = MC_FALSE;
   MC_T_REAL      fDeltaTime;

   MC_T_REAL      X[MAX_NUM_OF_SPLINE_POINTS];
   MC_T_REAL      Y[MAX_NUM_OF_SPLINE_POINTS];


    /* check input parameters */
    if(fTimeScale != 0.0)  bDoTimeScale = MC_TRUE;
    if(fPositionScale != 0.0) bDoPositionScale = MC_TRUE;

    if(!pTimePosition->bIsAbsolute)
    {
      fCommandedPos = pAxis->lCommandedPosition / (MC_T_REAL)pAxis->dwCalcIncPerMM;
    }

    /* setup control points */
    //std::vector<MC_T_REAL> X(pTimePosition->dwNumberOfPairs+1), Y(pTimePosition->dwNumberOfPairs+1);
    pMove->fTotalTime = 0;

    /* First Point at 0.0 */
    X[0] = 0;
    Y[0] = (fOffset+fCommandedPos);

    for(dwPairIndex = 0, pTp = pTimePosition->pTimePos; (dwPairIndex < pTimePosition->dwNumberOfPairs) && (wErrorID == MC_NO_ERROR); dwPairIndex++, pTp++)
    {
       X[dwPairIndex+1] = fDeltaTime = ((bDoTimeScale) ? pTp->fDeltaTime*fTimeScale : pTp->fDeltaTime) * 1E6;
       X[dwPairIndex+1] += pMove->fTotalTime;
       pMove->fTotalTime += fDeltaTime;

       Y[dwPairIndex+1] = (bDoPositionScale) ? (pTp->fPosition*fPositionScale) : pTp->fPosition;
       Y[dwPairIndex+1] += (fOffset+fCommandedPos);

       if(pAxis->bSWLimitPositiveEna)      /* positive limit enabled ? */
       {
           if(Y[dwPairIndex+1] * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM > pAxis->lSWLimitPositivePos)
           {
               wErrorID = MC_ERR_SW_LIMIT_POS;
           }
       }

       if(pAxis->bSWLimitNegativeEna)      /* negative limit enabled ? */
       {
           if(Y[dwPairIndex+1] * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM < pAxis->lSWLimitNegativePos)
           {
               wErrorID = MC_ERR_SW_LIMIT_NEG;
           }
       }
    }

    this->pSpline->set_points(pTimePosition->dwNumberOfPairs+1, X, Y);

    if (this->wErrorID != MC_NO_ERROR)
    {
       this->ChangeState(FBS_ERROR);
       return;
    }

    pMove->pProfile = this;
    pMove->fTimeInSpline = 0;
    pAxis->bPosProfile = EC_TRUE;
    pAxis->dwDirection = MC_DIR_POSITIVE;

   if(this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }
}

void MC_POSITION_PROFILE_T::DoState_Active()
{
   MC_T_BOOL bDoneLocal = MC_FALSE;

   if(pAxis->ePLCOpenState != MC_PLCOPEN_STATE_DISC_MOTION)
   {
      this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
      this->ChangeState(FBS_ERROR);
      return;
   }

   if (pAxis->oMove.fTimeInSpline >= pAxis->oMove.fTotalTime)
   {
       bDoneLocal = MC_TRUE;
   }

   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
      return;
   }

   if (bDoneLocal)
   {
      this->ChangeState(FBS_DONE);
      pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
      pAxis->oMove.pProfile = EC_NULL;
   }
}

MC_T_BOOL MC_POSITION_PROFILE_T::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ACTIVE)
         {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
         }

         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_ACTIVE:
      {
         DoState_Active();
         if (this->fbState == FBS_DONE)
         {
            this->bDone = MC_TRUE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            continueSm = MC_TRUE;
         }
         else if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
   case FBS_ERROR:
   case FBS_DONE:
      {
         /* 2.4.1 General rules - Output status
          * Done, Error, ErrorID and CommandAborted are reset with the falling edge
          * of Execute.
          * It must be guaranted that the coresponding outputs are set for at least
          * one cycle...
          */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bDone = MC_FALSE;
            this->bCommandAborted = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         break;
      }
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->bDone, this->bError, this->bCommandAborted));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bDone, this->bError, this->bCommandAborted));

   return continueSm;
}

void MC_POSITION_PROFILE_T::OnCycle()
{
   while (StateMachine());
}
#endif

/********************************************************************************/
/** \brief Set Position (3.17)

    This Function Block shifts the coordinate system of an axis by manipulating both
    the set-point position as well as the actual position of an axis with the same value
    without any movement caused. (Re-calibration with same following error).
    This can be used for instance for a reference situation. This Function Block can also be used
    during motion without changing the commanded position, which is now positioned in the
    shifted coordinate system.
 */
void MC_SET_POSITION_T::OnCycle()
{
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if (this->bLastExecute != this->bExecute)
    {
        /* any edge */
        this->bDone = MC_FALSE;
        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: Exe=%d Busy=%d Done=%d Error=%d\n",
            GetName(), this->bExecute, this->bBusy, this->bDone, this->bError));
    }

    if (!this->bDone)
    {
        /* rising edge --> set position */
        if (!this->bBusy && (this->bExecute && !this->bLastExecute))
        {
            this->bBusy = MC_TRUE;
            switch (this->dwExecMode)
            {
            case MC_IMMEDIATELY:
                // Set position
                MC_SetPosition(this->pAxis, this->fPosition, this->bRelative);

                this->bDone = MC_TRUE;
                break;

            case MC_QUEUED:
            default:
                this->bError = MC_TRUE;
                this->wErrorID = MC_ERR_PARA_NOT_SUPPORTED;
                break;
            }
        }
    }

    /* check "output exclusivity" */
    if (this->bDone || this->bError)
    {
        this->bBusy = MC_FALSE;
    }
#ifdef DEBUG
    if (this->bExecute)
    {
        if (this->bBusy) nTrueCnt++;
    }
    if (this->bDone) nTrueCnt++;
    if (this->bError) nTrueCnt++;
    OsDbgAssert(nTrueCnt <= 1);
#endif

    this->bLastExecute = this->bExecute;
}


/********************************************************************************/
/** \brief Read Parameter (3.19)

    This Function Block returns the value of a vendor specific parameter. The returned Value has to be converted to Real
    if necessary.
 */
void MC_READ_PARAMETER_T::OnCycle()
{
    MC_T_BOOL       bSupported = MC_TRUE;
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if(this->bEnable)
    {
        this->bBusy  = MC_FALSE;

        switch(this->nParameterNumber)
        {
        case MCFB_PN_COMMANDED_POSITION:
            if(pAxis->dwCalcIncPerMM != 0)
                this->fValue = pAxis->lCommandedPosition / (MC_T_REAL)pAxis->dwCalcIncPerMM;
            else
                this->fValue = 0;
            break;

        case MCFB_PN_SW_LIMIT_POS:
            if(pAxis->dwCalcIncPerMM != 0)
                this->fValue = pAxis->lSWLimitPositivePos / (MC_T_REAL)pAxis->dwCalcIncPerMM;
            else
                this->fValue = 0;
            break;

        case MCFB_PN_SW_LIMIT_NEG:
            if(pAxis->dwCalcIncPerMM != 0)
                this->fValue = pAxis->lSWLimitNegativePos / (MC_T_REAL)pAxis->dwCalcIncPerMM;
            else
                this->fValue = 0;
            break;

        case MCFB_PN_ACTUAL_VELOCITY:
            if(pAxis->fVelToInc != 0)
                this->fValue = pAxis->lActualVelocity / pAxis->fVelToInc;
            else
                this->fValue = 0;
            break;

        case MCFB_PN_ACTUAL_POSITION:
            if(pAxis->dwCalcIncPerMM != 0)
                this->fValue = pAxis->lActualPosition / (MC_T_REAL)pAxis->dwCalcIncPerMM;
            else
                this->fValue = 0;
            break;

        case MCFB_PN_COMMANDED_VELOCITY:
            if(pAxis->fVelToInc != 0)
            {
                this->fValue = pAxis->oMove.lCommandVel / pAxis->fVelToInc;
                if (pAxis->dwDirection == MC_DIR_NEGATIVE) this->fValue = -this->fValue;
            }
            else
                this->fValue = 0;
            break;

        case MCFB_PN_COMMANDED_ACCELERATION:
            if(pAxis->fAccToInc != 0)
                this->fValue = pAxis->oMove.lCommandAcc / pAxis->fAccToInc;
            else
                this->fValue = 0;
            break;

        case MCFB_PN_COMMANDED_JERK:
            if(pAxis->fJerkToInc != 0)
                this->fValue = pAxis->oMove.lCommandJerk / pAxis->fJerkToInc;
            else
                this->fValue = 0;
            break;

        default:
            bSupported = MC_FALSE;
            this->bError   = MC_TRUE;
            this->wErrorID = MC_ERR_PARA_NOT_SUPPORTED;
            break;
        }

        if(bSupported)
        {
            this->bValid = MC_TRUE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
        }
    }
    else
    {
        this->bValid = MC_FALSE;
        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
    }

    /* check "output exclusivity" */
#ifdef DEBUG
    if(this->bEnable)
    {
        if(this->bValid) nTrueCnt++;
        if(this->bError) nTrueCnt++;
        OsDbgAssert(nTrueCnt <= 1);
    }
#endif
}


/********************************************************************************/
/** \brief Read bool parameter (3.19)

    This Function Block returns the value of a vendor specific parameter.
 */
void MC_READ_BOOL_PARAMETER_T::OnCycle()
{
    MC_T_BOOL       bSupported = MC_TRUE;
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if(this->bEnable)
    {
        this->bBusy  = MC_FALSE;

        switch(this->nParameterNumber)
        {
        case MCFB_PN_ENA_LIMIT_POS:
            this->bValue = pAxis->bSWLimitPositiveEna;
            break;

        case MCFB_PN_ENA_LIMIT_NEG:
            this->bValue = pAxis->bSWLimitNegativeEna;
            break;

        default:
            bSupported = MC_FALSE;
            this->bError = MC_TRUE;
            this->wErrorID = MC_ERR_PARA_NOT_SUPPORTED;
            break;
        }

        if(bSupported)
        {
            this->bValid = MC_TRUE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
        }
    }
    else
    {
        this->bValid = MC_FALSE;
        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
    }

    /* check "output exclusivity" */
#ifdef DEBUG
    if(this->bEnable)
    {
        if(this->bValid) nTrueCnt++;
        if(this->bError) nTrueCnt++;
        OsDbgAssert(nTrueCnt <= 1);
    }
#endif
}


/********************************************************************************/
/** \brief Write Parameter (3.20)

    This Function Block modifies the value of a vendor specific parameter.
 */
void MC_WRITE_PARAMETER_T::OnCycle()
{
    MC_T_BOOL       bSupported = MC_TRUE;

    if(this->bExecute && !this->bLastExecute)
    {
        /* rising edge */
        this->bBusy  = MC_FALSE;

        switch(this->nParameterNumber)
        {
        case MCFB_PN_SW_LIMIT_POS:
            pAxis->lSWLimitPositivePos = (MC_T_POS_CALC)(this->fValue *  (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
            break;

        case MCFB_PN_SW_LIMIT_NEG:
            pAxis->lSWLimitNegativePos = (MC_T_POS_CALC)(this->fValue * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
            break;

        default:
            bSupported = MC_FALSE;
            this->bError = MC_TRUE;
            this->wErrorID = MC_ERR_PARA_NOT_SUPPORTED;
            break;
        }

        if(bSupported)
        {
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
        }
    }

    /* falling edge of execute */
    if(!this->bExecute && this->bLastExecute)
    {
        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
    }

    this->bLastExecute = this->bExecute;
}



/********************************************************************************/
/** \brief Write Bool Parameter (3.20)

    This Function Block modifies the value of a vendor specific parameter.
 */
void MC_WRITE_BOOL_PARAMETER_T::OnCycle()
{
    MC_T_BOOL       bSupported = MC_TRUE;

    if(this->bExecute && !this->bLastExecute)
    {
        /* rising edge */
        this->bBusy  = MC_FALSE;

        switch(this->nParameterNumber)
        {
        case MCFB_PN_ENA_LIMIT_POS:
            pAxis->bSWLimitPositiveEna = this->bValue;
            break;

        case MCFB_PN_ENA_LIMIT_NEG:
            pAxis->bSWLimitNegativeEna = this->bValue;
            break;

        default:
            bSupported = MC_FALSE;
            this->bError = MC_TRUE;
            this->wErrorID = MC_ERR_PARA_NOT_SUPPORTED;
            break;
        }

        if(bSupported)
        {
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
        }
    }

    /* falling edge of execute */
    if(!this->bExecute && this->bLastExecute)
    {
        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
    }

    this->bLastExecute = this->bExecute;
}


/********************************************************************************/
/** \brief Read digital input (3.21)

    This Function Block gives access to the value of the input.
    The application has to provide a DWORD pointer to input data by calling InitInputs() in advance.
    For a CiA402 drive typically the object 0x60FD has to be mapped into the PDO.
*/
void MC_READ_DIGITAL_INPUT_T::OnCycle()
{
    MC_T_BOOL       bSupported = MC_TRUE;
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if (this->bEnable)
    {
        this->bBusy = MC_FALSE;

        if ((this->nInputNumber >= 0) && (this->nInputNumber <= 31))
        {
            if (this->Axis->pdwDigitalInputs != EC_NULL)
            {
                this->bValue = (*this->Axis->pdwDigitalInputs & (1 << nInputNumber)) ? MC_TRUE : MC_FALSE;
            }
            else
            {
                bSupported = MC_FALSE;
                this->bError = MC_TRUE;
                this->wErrorID = MC_ERR_INVALID_INPUT_NUM;
            }
        }
        else
        {
            bSupported = MC_FALSE;
            this->bError = MC_TRUE;
            this->wErrorID = MC_ERR_INVALID_INPUT_NUM;
        }

        if (bSupported)
        {
            this->bValid = MC_TRUE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
        }
    }
    else
    {
        this->bValid = MC_FALSE;
        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
    }

    /* check "output exclusivity" */
#ifdef DEBUG
    if (this->bEnable)
    {
        if (this->bValid) nTrueCnt++;
        if (this->bError) nTrueCnt++;
        OsDbgAssert(nTrueCnt <= 1);
    }
#endif
}


/********************************************************************************/
/** \brief Read digital output (3.22)

    This Function Block gives access to the value of the output.
    The application has to provide a DWORD pointer to output data by calling InitOutputs() in advance.
    For a CiA402 drive typically the object 0x60FE has to be mapped into the PDO.
 */
void MC_READ_DIGITAL_OUTPUT_T::OnCycle()
{
    MC_T_BOOL       bSupported = MC_TRUE;
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if (this->bEnable)
    {
        this->bBusy = MC_FALSE;

        if ((this->nOutputNumber >= 0) && (this->nOutputNumber <= 31))
        {
            if (this->Axis->pdwDigitalOutputs != EC_NULL)
            {
                this->bValue = (*this->Axis->pdwDigitalOutputs & (1 << nOutputNumber)) ? MC_TRUE : MC_FALSE;
            }
            else
            {
                bSupported = MC_FALSE;
                this->bError = MC_TRUE;
                this->wErrorID = MC_ERR_INVALID_OUTPUT_NUM;
            }
        }
        else
        {
            bSupported = MC_FALSE;
            this->bError = MC_TRUE;
            this->wErrorID = MC_ERR_INVALID_OUTPUT_NUM;
        }

        if (bSupported)
        {
            this->bValid = MC_TRUE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
        }
    }
    else
    {
        this->bValid = MC_FALSE;
        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
    }

    /* check "output exclusivity" */
#ifdef DEBUG
    if (this->bEnable)
    {
        if (this->bValid) nTrueCnt++;
        if (this->bError) nTrueCnt++;
        OsDbgAssert(nTrueCnt <= 1);
    }
#endif
}


/********************************************************************************/
/** \brief Write digital output (3.23)

    This Function Block writes a value to the output.
    The application has to provide a DWORD pointer to output data by calling InitOutputs() in advance.
    For a CiA402 drive typically the object 0x60FE has to be mapped into the PDO.
 */
void MC_WRITE_DIGITAL_OUTPUT_T::OnCycle()
{
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if (this->bLastExecute != this->bExecute)
    {
        /* any edge */
        this->bDone = MC_FALSE;
        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: Exe=%d Busy=%d Done=%d Error=%d\n", GetName(),
             this->bExecute, this->bBusy, this->bDone, this->bError));
    }

    if (!this->bDone)
    {
        /* rising edge --> set position */
        if (!this->bBusy && (this->bExecute && !this->bLastExecute))
        {
            this->bBusy = MC_TRUE;

            switch (this->dwExecMode)
            {
            case MC_IMMEDIATELY:
                if ((this->nOutputNumber >= 0) && (this->nOutputNumber <= 31))
                {
                    if (this->Axis->pdwDigitalOutputs != EC_NULL)
                    {
                        if (this->bValue == MC_TRUE)
                        {
                            *this->Axis->pdwDigitalOutputs |= (1 << nOutputNumber);
                        }
                        else
                        {
                            *this->Axis->pdwDigitalOutputs &= ~(1 << nOutputNumber);
                        }
                        this->bDone = MC_TRUE;
                    }
                    else
                    {
                        this->bError = MC_TRUE;
                        this->wErrorID = MC_ERR_INVALID_OUTPUT_NUM;
                    }
                }
                else
                {
                    this->bError = MC_TRUE;
                    this->wErrorID = MC_ERR_INVALID_OUTPUT_NUM;
                }
                break;

            case MC_QUEUED:
            default:
                this->bError = MC_TRUE;
                this->wErrorID = MC_ERR_PARA_NOT_SUPPORTED;
                break;
            }
        }
    }

    /* check "output exclusivity" */
    if (this->bDone || this->bError)
    {
        this->bBusy = MC_FALSE;
    }
#ifdef DEBUG
    if (this->bExecute)
    {
        if (this->bBusy) nTrueCnt++;
    }
    if (this->bDone) nTrueCnt++;
    if (this->bError) nTrueCnt++;
    OsDbgAssert(nTrueCnt <= 1);
#endif

    this->bLastExecute = this->bExecute;
}


/********************************************************************************/
/** \brief MC_ReadActualPosition returns the current position of an axis (3.24)
*
* \
*/
void MC_READ_ACTUAL_POSITION_T::OnCycle()
{
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if(this->bEnable)
    {
        this->fPosition = pAxis->lActualPosition / (MC_T_REAL)pAxis->dwCalcIncPerMM;
        this->bValid = MC_TRUE;

        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
    }
    else
    {
        this->fPosition = 0;
        this->bValid = MC_FALSE;
    }

    /* check "output exclusivity" */
#ifdef DEBUG
    if(this->bEnable)
    {
        if(this->bValid) nTrueCnt++;
        if(this->bError) nTrueCnt++;
        OsDbgAssert(nTrueCnt <= 1);
    }
#endif
}


/********************************************************************************/
/** \brief MC_ReadActualVelocity returns the current position of an axis (3.25)
*
* \return N/A
*/
void MC_READ_ACTUAL_VELOCITY_T::OnCycle()
{
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if(this->bEnable)
    {
        this->fVelocity = pAxis->lActualVelocity / pAxis->fVelToInc;
        this->bValid = MC_TRUE;
        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
    }
    else
    {
        this->fVelocity = 0;
        this->bValid = MC_FALSE;
    }

    /* check "output exclusivity" */
#ifdef DEBUG
    if(this->bEnable)
    {
        if(this->bValid) nTrueCnt++;
        if(this->bError) nTrueCnt++;
        OsDbgAssert(nTrueCnt <= 1);
    }
#endif
    return;
}


/***************************************************************************************************/
/** \brief  MC_ReadMotionState Read motion state (3.28)

This Function Block returns in detail the status of the axis with respect
to the motion currently in progress.
*/
void MC_READ_MOTION_STATE_T::OnCycle()
{
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if(this->bEnable)
    {
        this->bValid   = MC_TRUE;
        this->bError   = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;

        if(pAxis->oMove.eTrajectState != eTrajStatus_Idle)
        {
            this->bDirPositive = (pAxis->dwDirection == MC_DIR_POSITIVE);
            this->bDirNegative = (pAxis->dwDirection == MC_DIR_NEGATIVE);
        }
        else
        {
            this->bDirPositive = MC_FALSE;
            this->bDirNegative = MC_FALSE;
        }

        //if(this->eSource)     /* currently not supported */
        this->bConstantVelocity = (pAxis->oMove.eTrajectState == eTrajStatus_Vel);
        this->bAccelerating     = (pAxis->oMove.eTrajectState == eTrajStatus_Acc);
        this->bDecelerating     = (pAxis->oMove.eTrajectState == eTrajStatus_Dec);
    }
    else
    {
        /* do nothing */
        this->bValid   = MC_FALSE;
    }

    /* check "output exclusivity" */
#ifdef DEBUG
    if(this->bEnable)
    {
        if(this->bValid) nTrueCnt++;
        if(this->bError) nTrueCnt++;
        OsDbgAssert(nTrueCnt <= 1);
    }
#endif
}

/***************************************************************************************************/
/** \brief  MC_ReadAxisInfo Read axis info (3.29)

This Function Block reads information concerning an axis, like modes, inputs directly related to the axis, and certain
status information.
*/
void MC_READ_AXIS_INFO_T::OnCycle()
{
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if (this->bEnable)
    {
        this->bValid = MC_TRUE;
        this->bSimulation = pAxis->eAxisType != MC_AXIS_TYPE_REAL_ALL;
        this->nVerbose = pAxis->LogParms.dwLogLevel;
        this->ePLCOpenState = pAxis->ePLCOpenState;

        if (pAxis->eAxisType == MC_AXIS_TYPE_REAL_ALL)
        {
           switch (pAxis->eProfile)
           {
           case MC_T_AXIS_PROFILE_DS402:
              this->nDriveState = pAxis->eDS402State;
              break;
           case MC_T_AXIS_PROFILE_SERCOS:
              this->nDriveState = pAxis->eSercosState;
              break;
           case MC_T_AXIS_PROFILE_NONE:
           default:
               this->bValid = MC_FALSE;
               this->bError = MC_TRUE;
               this->wErrorID = MC_ERR_NO_ECAT_PROFILE;
               break;
           }
        }
        else
        {
           this->nDriveState = pAxis->eSimuState;
        }

        this->bDriveErrorAckReq = pAxis->bDriveErrorAckReq;
        if (this->bDriveErrorAck) pAxis->bDriveErrorAckReq = MC_FALSE;

        this->fCommandedPosition = pAxis->lCommandedPosition / (MC_T_REAL)pAxis->dwCalcIncPerMM;
    }
    else
    {
        /* do nothing */
        this->bValid   = MC_FALSE;
    }

    /* check "output exclusivity" */
#ifdef DEBUG
    if(this->bEnable)
    {
        if(this->bValid) nTrueCnt++;
        if(this->bError) nTrueCnt++;
        OsDbgAssert(nTrueCnt <= 1);
    }
#endif
}


/***************************************************************************************************/
/** \brief  Read axis error (3.30)

This Function Block presents general axis errors not relating to the Function Blocks.
(for instance axis errors, drive errors, communication errors)
*/
void MC_READ_ERROR_T::OnCycle()
{
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    if(this->bEnable)
    {
        this->bValid   = MC_TRUE;
        if(pAxis->bError)
        {
            this->bError   = pAxis->bError;
            this->wErrorID = pAxis->wErrorID;
        }
        else
        {
            this->bError   = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
        }
    }
    else
    {
        /* do nothing */
        this->bValid   = MC_FALSE;
    }

    /* check "output exclusivity" */
#ifdef DEBUG
    if(this->bEnable)
    {
        if(this->bValid) nTrueCnt++;
        if(this->bError) nTrueCnt++;
        OsDbgAssert(nTrueCnt <= 1);
    }
#endif
}


/********************************************************************************/
/** \brief MC_Reset (3.31)
*
* This FB makes the transition from the state ErrorStop to StandStill by resetting all
* internal axis-related errors - it does not effect the output of the FB instances.
*
* \return N/A
*/
void MC_RESET_T::OnCycle()
{
#ifdef DEBUG
    EC_T_INT        nTrueCnt = 0;
#endif

    /* 'Execute' edge detection */
    if(this->bExecute && !this->bLastExecute)
    {
        /* rising edge */
        this->bBusy = MC_TRUE;
    }
    else if (!this->bExecute && this->bLastExecute)
    {
        /* falling edge */
        this->bDone = MC_FALSE;
    }
    if(this->bLastExecute != this->bExecute)
    {
        /* any edge */
        this->bError = MC_FALSE;
        this->wErrorID = MC_NO_ERROR;
    }
    this->bLastExecute = this->bExecute;

    if(!this->bDone)
    {
        if(this->bBusy)
        {
            if(pAxis->ePLCOpenState == MC_PLCOPEN_STATE_ERROR_STOP)
            {
                /* clear all errors */
                pAxis->SetAxisError(MC_NO_ERROR);

                if(!pAxis->bMcPowerStatus)
                {
                    pAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISABLED;   /* state machine, note 3 */
                }
                else
                {
                    if(pAxis->bMcPowerEnable)
                    {
                        pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;    /* state machine, note 4 */
                    }
                    else
                    {
                        pAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISABLED;
                    }
                }

                this->bDone = MC_TRUE;
                this->bBusy = MC_FALSE;
            }
            else
            {
                this->bError = MC_TRUE;
                this->bBusy  = MC_FALSE;
                this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
            }
        }
    }

    /* check "output exclusivity" */
#ifdef DEBUG
    if(this->bExecute)
    {
        if(this->bBusy) nTrueCnt++;
    }
    if(this->bDone) nTrueCnt++;
    if(this->bError) nTrueCnt++;
    OsDbgAssert(nTrueCnt <= 1);
#endif
    return;
}

#ifdef EC_MOTION_SUPPORT_MC_CAMMING
/********************************************************************************/
/** \brief MC_CamTableSelect  (4.2)

This Function Block selects the CAM tables by setting the connections to the relevant tables

*/


MC_CAMTABLE_SELECT_T::MC_CAMTABLE_SELECT_T(MC_T_AXIS_REF *pAxis, MC_T_AXIS_REF *pMasterAxisParm, MC_T_CAM_REF *pCamTableParm)
: // Init from constructor
MC_FB_T(pAxis, MCFB_ID_CAMTABLE_SELECT),
// Init external OUT's
Done(static_cast<const MC_T_BOOL &>(bDone)),
Busy(static_cast<const MC_T_BOOL &>(bBusy)),
CamTableIDPtr(static_cast<const MC_T_P_CAM_ID &>(pCamTableID)),
// Init variables
Execute(0),
Periodic(0),
MasterAbsolute(0),
SlaveAbsolute(0),
ExecutionMode(MC_IMMEDIATELY),
MasterAxis(pMasterAxisParm),
CamTable(pCamTableParm),
pMasterAxis(MasterAxis),
pCamTable(CamTable),
bDone(0),
bBusy(0),
bExecute(static_cast<const MC_T_BOOL &>(Execute))
{
    // Create Spline
    this->pSpline = EC_NEW(MC_T_SPLINE);
    OsDbgAssert(this->pSpline != EC_NULL);
}

MC_CAMTABLE_SELECT_T::~MC_CAMTABLE_SELECT_T()
{
    // Delete Spline
    delete(this->pSpline);
}


void MC_CAMTABLE_SELECT_T::StateTrans_Execute()
{
    if((this->Axis == EC_NULL) || (this->MasterAxis == EC_NULL))
    {
        this->wErrorID = MC_ERR_INVALID_PARAMETER;
        this->ChangeState(FBS_ERROR);
        return;
    }

    if(this->CamTable == EC_NULL)
    {
        this->wErrorID = MC_ERR_INVALID_PARAMETER;
        this->ChangeState(FBS_ERROR);
        EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "%s: Error CamTable not defined \n", GetName()));
        return;
    }

    this->ChangeState(FBS_BUSY);
}

void MC_CAMTABLE_SELECT_T::DoState_Busy()
{
    MC_T_REAL      afMasPos[MAX_NUM_OF_SPLINE_POINTS];
    MC_T_REAL      afSlaPos[MAX_NUM_OF_SPLINE_POINTS];

    /* check input parameters */
    if(MasterAxis == EC_NULL)
    {
        this->wErrorID = MC_ERR_MASTER_AXIS_INVALID;
        this->ChangeState(FBS_ERROR);
        return;
    }

    if(CamTable == EC_NULL)
    {
        this->wErrorID = MC_ERR_CAM_TABLE_ID_INVALID;
        this->ChangeState(FBS_ERROR);
        return;
    }

    if(CamTable->nNumOfElements < 2)     /* check number of elements */
    {
        this->wErrorID = MC_ERR_CAM_TABLE_ELEM_TOO_LESS;
        this->ChangeState(FBS_ERROR);
        return;
    }

    if(CamTable->nNumOfElements > MAX_NUM_OF_SPLINE_POINTS)
    {
        this->wErrorID = MC_ERR_CAM_TABLE_ELEM_TOO_MANY;
        this->ChangeState(FBS_ERROR);
        return;
    }

    switch(CamTable->eVarType)          /* check variable type */
    {
    case MC_CAM_VAR_TYPE_INT:           /* table contains MC_T_INT values   */
        break;
    case MC_CAM_VAR_TYPE_REAL:          /* table contains MC_T_REAL values */
        break;
    default:
        this->wErrorID = MC_ERR_CAM_TABLE_VARTYPE_INVALID;
        this->ChangeState(FBS_ERROR);
        return;
    }

    if(this->pSpline == EC_NULL)
    {
        this->wErrorID = MC_ERR_CAM_SPLINE_INVALID;
        this->ChangeState(FBS_ERROR);
        return;
    }

    if(CamTable->pData == EC_NULL)
    {
        this->wErrorID = MC_ERR_CAM_TABLE_DATA_INVALID;
        this->ChangeState(FBS_ERROR);
        return;
    }

    oCamTableID.MasterAxis = MasterAxis;
    oCamTableID.SlaveAxis = Axis;
    oCamTableID.CamRef = CamTable;
    oCamTableID.Periodic = Periodic;
    oCamTableID.MasterAbsolute = MasterAbsolute;
    oCamTableID.SlaveAbsolute = SlaveAbsolute;
    oCamTableID.pSpline = this->pSpline;
    this->pCamTableID = &oCamTableID;

    {

        MC_T_INT i;
        if(CamTable->eVarType == MC_CAM_VAR_TYPE_INT)
        {
            MC_T_INT*   pnValue;
            for(i = 0, pnValue = (MC_T_INT*)CamTable->pData; i < CamTable->nNumOfElements; i++)
            {
                afMasPos[i] = (MC_T_REAL)*pnValue++;
                afSlaPos[i] = (MC_T_REAL)*pnValue++;
            }
        }
        else if(CamTable->eVarType == MC_CAM_VAR_TYPE_REAL)
        {
            MC_T_REAL*   pfValue;
            for(i = 0, pfValue = (MC_T_REAL*)CamTable->pData; i < CamTable->nNumOfElements; i++)
            {
                afMasPos[i] = *pfValue++;
                afSlaPos[i] = *pfValue++;
            }
        }
        oCamTableID.fMasterStart = afMasPos[0];
        oCamTableID.fMasterEnd = afMasPos[CamTable->nNumOfElements-1];
        oCamTableID.fMasterPeriod = oCamTableID.fMasterEnd - oCamTableID.fMasterStart;

        if(oCamTableID.fMasterPeriod == 0)
        {
            this->wErrorID = MC_ERR_CAM_IN_MAS_PERIOD_ZERO;
            this->ChangeState(FBS_ERROR);
        }
        else if(oCamTableID.fMasterPeriod < 0)
        {
            this->wErrorID = MC_ERR_CAM_IN_MAS_PERIOD_ZERO;
            this->ChangeState(FBS_ERROR);
        }
    }

    /* calculate the curve */
    switch(CamTable->eInterpolType)
    {
    case MC_CAM_INTERPOL_TYPE_LIN:           /* interpolation type linear   */
        this->wErrorID = this->pSpline->set_points(CamTable->nNumOfElements, afMasPos, afSlaPos, EC_FALSE);
        break;
    case MC_CAM_INTERPOL_TYPE_CUB:         /* interpolation type cubic */
        this->wErrorID = this->pSpline->set_points(CamTable->nNumOfElements, afMasPos, afSlaPos, EC_TRUE);
        break;
    default:
        this->wErrorID = MC_ERR_CAM_TABLE_INTERPOL_INVALID;
        this->ChangeState(FBS_ERROR);
        return;
    }

   if (this->wErrorID != MC_NO_ERROR)
   {
      this->ChangeState(FBS_ERROR);
   }
   else
   {
      this->ChangeState(FBS_DONE);
   }
}


MC_T_BOOL MC_CAMTABLE_SELECT_T::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
        if (this->fbState == FBS_DONE)
        {
            this->bDone = MC_TRUE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
        }
        else if (this->fbState == FBS_ERROR)
        {
            this->bError = MC_TRUE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
        }
        break;
    }

   case FBS_ERROR:
   case FBS_DONE:
      {
         /* 2.4.1 General rules - Output status
          * Done, Error, ErrorID and CommandAborted are reset with the falling edge
          * of Execute.
          * It must be guaranted that the coresponding outputs are set for at least
          * one cycle...
          */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bDone = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED:
   default:
       break;
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->bDone, this->bError));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bDone, this->bError));

   return continueSm;
}

void MC_CAMTABLE_SELECT_T::OnCycle()
{
   while (StateMachine());
}


/********************************************************************************/
/** \brief This Function Block engages the CAM (4.3)

*/
void MC_CAM_IN_T::StateTrans_Execute()
{

    /* check input parameters */
    if((pAxis == EC_NULL) || (this->pMasterAxis == EC_NULL))
    {
        this->wErrorID = MC_ERR_MASTER_AXIS_INVALID;
        this->ChangeState(FBS_ERROR);
        return;
    }

    /* is there a valid CAM table ? */
    if((pCamTableID == EC_NULL) || (pCamTableID->CamRef == EC_NULL))
    {
        this->wErrorID = MC_ERR_CAM_TABLE_ID_INVALID;
        this->ChangeState(FBS_ERROR);
        return;
    }

    /* check start mode */
    switch(this->eStartMode)
    {
    case MC_SM_ABSOLUTE:            /* Absolute */
        break;
    case MC_SM_RELATIVE:            /* Relative */
        break;
    case MC_SM_RAMP_IN:             /* Ramp In */
        break;

    case MC_SM_RAMP_IN_POS:         /* Ramp In only positive */
    case MC_SM_RAMP_IN_NEG:         /* Ramp In only negative */
    default:
        this->wErrorID = MC_ERR_START_MODE_INVALID;
        break;
    }

    if (this->wErrorID == MC_NO_ERROR)
    {
        this->wErrorID = HandleBlending(pAxis, this, this->dwBufferMode, 0, 0.0, EC_NULL);
    }

    if (this->wErrorID != MC_NO_ERROR)
    {
        this->ChangeState(FBS_ERROR);
        return;
    }

    this->ChangeState(FBS_BUSY);
}


void MC_CAM_IN_T::DoState_Busy()
{
    switch (pAxis->ePLCOpenState)        /* check state of master axis */
    {
    case MC_PLCOPEN_STATE_STAND_STILL:
    case MC_PLCOPEN_STATE_DISC_MOTION:
    case MC_PLCOPEN_STATE_CONT_MOTION:
        {
            MC_T_BOOL fbActive = false;
            if(this->dwBufferMode == MC_ABORTING)
            {
                MC_AbortCommand(pAxis, MCFB_ID_CAM_IN);     /* abort other MCFBs */
            }

            this->wErrorID = MC_BufferActivateNext(pAxis, this, &fbActive);
            if (this->wErrorID == MC_NO_ERROR)
            {
                if(fbActive)
                {
                    /* check if slave axis is in standstill or synchronized motion */
                    if((pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STAND_STILL) ||
                       (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_SYNC_MOTION))
                    {
                        /* link the two axis with pointers */
                        this->pAxis->pSyncMasterAxis = pMasterAxis;

                        /* store parameters in CAM Table */
                        this->pCamTableID->fMasterOffset = this->fMasterOffset;
                        this->pCamTableID->fSlaveOffset = this->fSlaveOffset;

                        this->pCamTableID->fMasterScaling = (this->fMasterScaling != 0) ? this->fMasterScaling : 1.0;
                        this->pCamTableID->fSlaveScaling  = (this->fSlaveScaling != 0)  ? this->fSlaveScaling  : 1.0;
                        this->pCamTableID->bEndOfProfile = MC_FALSE;

                        /* link this CAM table to slave axis */
                        this->pAxis->pCamTableID = this->pCamTableID;

                        /* this FB is now active */
                        this->ChangeState(FBS_ACTIVE);
                    }
                    else
                    {
                        this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE_SLAVE;
                        this->ChangeState(FBS_ERROR);
                    }
                }
            }
            else
            {
                this->ChangeState(FBS_ERROR);
            }
            break;
        }
    case MC_PLCOPEN_STATE_STOPPING:
        {
            /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
            * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
            */
            this->wErrorID = MC_ERR_BUSY_STOPPING;
            this->ChangeState(FBS_ERROR);
            break;
        }
    default:
        {
            this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
            this->ChangeState(FBS_ERROR);
        }
    }
}

void MC_CAM_IN_T::StateTrans_Active()
{
    MC_T_MOVEMENT*  pMove = &pAxis->oMove;
    MC_T_POS_CALC   lTargetPosition = 0;
    MC_T_BOOL       bMoveSlaveToSyncPos = EC_FALSE;
    MC_T_REAL       fMasterPos;

    /* use absolute or relative master coordinates? */
    if(pCamTableID->MasterAbsolute)
    {
        /* absolute coordinates */
        pAxis->pCamTableID->fMasterStartOffset = 0;
        fMasterPos = pMasterAxis->lCommandedPosition / (MC_T_REAL)pMasterAxis->dwCalcIncPerMM;
        fMasterPos = (fMasterPos*fMasterScaling) + this->fMasterOffset;

        /* check if master axis is in valid range */
        if(fMasterPos < pCamTableID->fMasterStart)     /* start position master */
        {
            this->wErrorID = MC_ERR_CAM_IN_MASPOS_TOO_SMALL;
            this->ChangeState(FBS_ERROR);
            return;
        }

        if(fMasterPos > pCamTableID->fMasterEnd)       /* end position master */
        {
            this->wErrorID = MC_ERR_CAM_IN_MASPOS_TOO_BIG;
            this->ChangeState(FBS_ERROR);
            return;
        }
    }
    else
    {
        /* relative coordinates */
        /* store current position in fMasterStartOffset */
        if(pCamTableID->fMasterStart == 0)
        {
            pAxis->pCamTableID->fMasterStartOffset = (pMasterAxis->lCommandedPosition / (MC_T_REAL)pMasterAxis->dwCalcIncPerMM);
        }
        else
        {
            this->wErrorID = MC_ERR_CAM_IN_MASPOS_NULL_MISSING;
            this->ChangeState(FBS_ERROR);
            return;
        }
    }

    /* if not absolute, save current position as SlaveOffset */
    pAxis->pCamTableID->fSlaveStartOffset = (pCamTableID->SlaveAbsolute) ? 0 : (pAxis->lCommandedPosition / (MC_T_REAL)pAxis->dwCalcIncPerMM);

    /* dispatch StartMode */
    switch(this->eStartMode)
    {
    case MC_SM_ABSOLUTE:            /* Absolute */
        /* sync immediately: slave axis may jump */
        MC_InitMovement(pMove, EC_FALSE, 0);
        break;

    case MC_SM_RELATIVE:            /* Relative */
        /* sync immediately but do some offset calc: slave axis may jump */
        MC_InitMovement(pMove, EC_FALSE, 0);

        /* always use start offset independend of SlaveAbsolute */
        pAxis->pCamTableID->fSlaveStartOffset = (pAxis->lCommandedPosition / (MC_T_REAL)pAxis->dwCalcIncPerMM);
        break;

    case MC_SM_RAMP_IN:             /* Ramp In */
        if(pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STAND_STILL)
        {
            MC_T_REAL     fTargetPos;
            MC_T_BOOL     bValidSlavePos = EC_FALSE;

            /* actual master pos */
            fMasterPos = pMasterAxis->lCommandedPosition / (MC_T_REAL)pMasterAxis->dwCalcIncPerMM;
            this->wErrorID = pAxis->pCamTableID->CalcSlavePos(fMasterPos, &fTargetPos, &bValidSlavePos);
            if(this->wErrorID != MC_NO_ERROR)
            {
                this->ChangeState(FBS_ERROR);
                return;
            }

            if(!bValidSlavePos)
            {
                this->ChangeState(FBS_ERROR);
                return;
            }
            lTargetPosition = (MC_T_POS_CALC)(fTargetPos * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
            bMoveSlaveToSyncPos = MC_TRUE;
        }
        else
        {
            this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
        }
        break;

    case MC_SM_RAMP_IN_POS:            /* Ramp In only positive */
    case MC_SM_RAMP_IN_NEG:            /* Ramp In only negative */
    default:
        this->wErrorID = MC_ERR_START_MODE_INVALID;
        break;
    }

    if(bMoveSlaveToSyncPos)
    {
        MC_T_POS_CALC   lDistance = 0;

        if (lTargetPosition > pAxis->lCommandedPosition)
        {
            /* are we moving into the wrong direction ? */
            if ((pAxis->dwDirection != MC_DIR_POSITIVE) && (pMove->lCommandVel != 0))
            {
                this->wErrorID = MC_ERR_DIR_CHANGED;
                this->ChangeState(FBS_ERROR);
                return;
            }

            lDistance = lTargetPosition - pAxis->lCommandedPosition;
            pAxis->dwDirection = MC_DIR_POSITIVE;
        }
        else
        {
            /* are we moving into the wrong direction ? */
            if ((pAxis->dwDirection != MC_DIR_NEGATIVE) && (pMove->lCommandVel != 0))
            {
                this->wErrorID = MC_ERR_DIR_CHANGED;
                this->ChangeState(FBS_ERROR);
                return;
            }

            lDistance = pAxis->lCommandedPosition - lTargetPosition;
            pAxis->dwDirection = MC_DIR_NEGATIVE;
        }

        /* SW limits enabled ? */
        if (pAxis->bSWLimitNegativeEna || pAxis->bSWLimitPositiveEna)
        {
            this->wErrorID = MC_CheckSoftwareLimits(pAxis, lDistance, EC_NULL);
            if (this->wErrorID != MC_NO_ERROR)
            {
                this->ChangeState(FBS_ERROR);
                return;
            }
        }

        pMove->lTotalDist = lDistance;

        /* calculate movement and setup parameters */
        this->wErrorID = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_TRUE, MC_FALSE, pMove->lTotalDist,
            this->fVelocity, pAxis->pWorkFb->fEndVel, this->fAcceleration, this->fDeceleration);

    }

    if(this->wErrorID != MC_NO_ERROR)
    {
        this->ChangeState(FBS_ERROR);
    }
    else
    {
        this->pAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISC_MOTION;
    }
}


void MC_CAM_IN_T::DoState_Active()
{
    MC_T_BOOL bDone = MC_FALSE;

    if(pAxis->ePLCOpenState != MC_PLCOPEN_STATE_DISC_MOTION)
    {
        this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE_SLAVE;
        this->ChangeState(FBS_ERROR);
        return;
    }

    /* check if sync position is reached */
    this->wErrorID = MC_CheckTargetPosReached(pAxis, &bDone);

    if (bDone)
    {
        this->ChangeState(FBS_DONE);

        /* now slave axis is synchronized with master axis */
        pAxis->dwDirection = MC_DIR_POSITIVE;
        pAxis->ePLCOpenState = MC_PLCOPEN_STATE_SYNC_MOTION;
        pAxis->eSetValSource = SETVAL_MASTER_CAMM;

        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: Exe=%d Busy=%d Active=%d InSync=%d CmdAborted=%d Error=%d\n",
          GetName(), this->bExecute, this->bBusy, this->bActive, this->bInSync, this->bCommandAborted, this->bError));
    }
}

MC_T_BOOL MC_CAM_IN_T::StateMachine()
{
   MC_T_BOOL continueSm = MC_FALSE; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ACTIVE)
         {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
         }

         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_ACTIVE:
      {
         DoState_Active();
         if (this->fbState == FBS_DONE)
         {
            this->bInSync = MC_TRUE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            continueSm = MC_TRUE;
         }
         else if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
   case FBS_ERROR:
   case FBS_DONE:
      {
         /* 2.4.1 General rules - Output status
          * Done, Error, ErrorID and CommandAborted are reset with the falling edge
          * of Execute.
          * It must be guaranted that the coresponding outputs are set for at least
          * one cycle...
          */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bInSync = MC_FALSE;
            this->bCommandAborted = MC_FALSE;
            this->bEndOfProfile = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         else
         {
             if(pAxis->pCamTableID != EC_NULL)
                this->bEndOfProfile = pAxis->pCamTableID->bEndOfProfile;
         }
         break;
      }
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->bInSync, this->bError, this->bCommandAborted));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bInSync, this->bError, this->bCommandAborted));

   return continueSm;
}

void MC_CAM_IN_T::OnCycle()
{
   while (StateMachine());
}


/********************************************************************************/
/** \brief This Function Block disengages the Slave axis from the Master axis immediately (4.4)

*/
void MC_CAM_OUT_T::StateTrans_Execute()
{
    this->ChangeState(FBS_BUSY);
}

void MC_CAM_OUT_T::DoState_Busy()
{
    switch (pAxis->ePLCOpenState)
    {
    case MC_PLCOPEN_STATE_SYNC_MOTION:
    case MC_PLCOPEN_STATE_ERROR_STOP:
        {
            /* unbind slave from master */
            pAxis->eSetValSource = pAxis->eSetValSourceOrg;
            pAxis->pSyncMasterAxis = EC_NULL;
            pAxis->pCamTableID = EC_NULL;

            /* this FB is now active */
            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISC_MOTION;
            this->ChangeState(FBS_ACTIVE);
            break;
        }
    case MC_PLCOPEN_STATE_STOPPING:
        {
            /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
            * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
            */
            this->wErrorID = MC_ERR_BUSY_STOPPING;
            this->ChangeState(FBS_ERROR);
            break;
        }
    default:
        {
            this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
            this->ChangeState(FBS_ERROR);
        }
    }
}


void MC_CAM_OUT_T::StateTrans_Active()
{
    MC_T_MOVEMENT*  pMove = &pAxis->oMove;

    if (pMove->lCommandVel == 0) return; // Brake velocity must be set

    if(this->fDeceleration == 0)
    {
        this->wErrorID = MC_ERR_DEC_OUT_OF_RANGE;
        this->ChangeState(FBS_ERROR);
    }

    this->wErrorID = CalcBrakeRamp(pAxis, this->fDeceleration, this->fJerk);
    if (this->wErrorID != MC_NO_ERROR)
    {
        this->ChangeState(FBS_ERROR);
        return;
    }
}


void MC_CAM_OUT_T::DoState_Active()
{
    MC_T_BOOL bDoneLocal = MC_FALSE;

    if (!((pAxis->ePLCOpenState == MC_PLCOPEN_STATE_DISC_MOTION) ||
        (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STAND_STILL) ||
        (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_HOMING)))
    {
        this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
        this->ChangeState(FBS_ERROR);
        return;
    }

    /* check if done: axis stopped */
    this->wErrorID = MC_CheckTargetPosReached(pAxis, &bDoneLocal);
    if (this->wErrorID != MC_NO_ERROR)
    {
        this->ChangeState(FBS_ERROR);
        return;
    }

    if (bDoneLocal)
    {
        pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
        this->ChangeState(FBS_DONE);

        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: Exe=%d Busy=%d Active=%d Done=%d Error=%d\n",
            GetName(), this->bExecute, this->bBusy, this->bActive, this->bDone, this->bError));
    }
}


MC_T_BOOL MC_CAM_OUT_T::StateMachine()
{
    MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

    switch (this->fbState)
    {
    case FBS_IDLE:
    {
        if (this->bExecute)
        {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
                this->bError = MC_TRUE;
                continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
                this->bBusy = MC_TRUE;
                continueSm = MC_TRUE;
            }
        }
        break;
    }
    case FBS_BUSY:
    {
        DoState_Busy();
        if (this->fbState == FBS_ACTIVE)
        {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
        }

        if (this->fbState == FBS_ERROR)
        {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
        }
        break;
    }
    case FBS_ACTIVE:
    {
        DoState_Active();
        if (this->fbState == FBS_DONE)
        {
            this->bDone = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
        }
        else if (this->fbState == FBS_ERROR)
        {
            this->bError = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
        }
        break;
    }
    case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
    case FBS_ERROR:
    case FBS_DONE:
    {
        /* 2.4.1 General rules - Output status
         * Done, Error, ErrorID and CommandAborted are reset with the falling edge
         * of Execute.
         * It must be guaranted that the coresponding outputs are set for at least
         * one cycle...
         */
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
        if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
        {
            this->bDone = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
        }
        break;
    }
    }

    /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
     * exclusive.
     */
    OsDbgAssert(IsExclusive(this->bBusy, this->bDone, this->bError));
    /* ... If Execute is true, one of these outputs has to be true. */
    OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bDone, this->bError));

    return continueSm;
}

void MC_CAM_OUT_T::OnCycle()
{
   while (StateMachine());
}

#endif /* EC_MOTION_SUPPORT_MC_CAMMING */

#ifdef EC_MOTION_SUPPORT_MC_GEARING

/*******************************************************************************************************/
/** \brief This Function Block commands a ration between the VELOCITY of a slave and master axis (4.5)

*/
void MC_GEAR_IN_T::StateTrans_Execute()
{

    /* check input parameters */
    if ((pAxis == EC_NULL) || (this->pMasterAxis == EC_NULL))
    {
        this->wErrorID = MC_ERR_MASTER_AXIS_INVALID; 
        this->ChangeState(FBS_ERROR);
        return;
    }

    /* is there a valid gearing data ? */
    if ((this->nRatioDenominator == 0) || (this->nRatioNumerator == 0))
    {
        this->wErrorID = MC_ERR_GEAR_PARA_INVALID;
        this->ChangeState(FBS_ERROR);
        return;
    }

    if ( !((this->eMasterValueSource == MC_SOURCE_SET_VALUE) || (eMasterValueSource == MC_SOURCE_ACTUAL_VALUE)))
    {
        this->wErrorID = MC_ERR_GEAR_PARA_INVALID;
        this->ChangeState(FBS_ERROR);
        return;
    }

/*  if (this->wErrorID == MC_NO_ERROR)
    {
        this->wErrorID = HandleBlending(pAxis, this, this->dwBufferMode, 0, 0.0, EC_NULL);
    } */

    if (this->wErrorID != MC_NO_ERROR)
    {
        this->ChangeState(FBS_ERROR);
        return;
    }

    this->ChangeState(FBS_BUSY);
}


void MC_GEAR_IN_T::DoState_Busy()
{
    switch (pAxis->ePLCOpenState)        /* check state of master axis */
    {
    case MC_PLCOPEN_STATE_STAND_STILL:
    case MC_PLCOPEN_STATE_SYNC_MOTION:
    {
        MC_T_BOOL fbActive = MC_TRUE;
        MC_AbortCommand(pAxis, MCFB_ID_GEAR_IN);     /* abort other MCFBs */

        if (this->wErrorID == MC_NO_ERROR)
        {
            if (fbActive)
            {
                /* if this axis isn´t already synchronzed, that master axis has to be in standstill */
                if (pAxis->ePLCOpenState != MC_PLCOPEN_STATE_SYNC_MOTION)
                {
                    if (pMasterAxis->ePLCOpenState != MC_PLCOPEN_STATE_STAND_STILL)
                    {
                        this->wErrorID = MC_ERR_GEAR_INVALID_MAS_STATE;
                        this->ChangeState(FBS_ERROR);
                    }
                }
                if (this->wErrorID == MC_NO_ERROR)
                {
                    /* link the two axis with pointers */
                    this->pAxis->eSetValSource = SETVAL_MASTER_GEAR;
                    this->pAxis->pSyncMasterAxis = pMasterAxis;

                    /* store parameters */
                    this->pAxis->oGearingData.nRatioNumerator = this->nRatioNumerator;
                    this->pAxis->oGearingData.dwRatioDenominator = this->nRatioDenominator;
                    this->pAxis->oGearingData.eMasterValueSource = this->eMasterValueSource;
                    this->pAxis->oMove.fSlaveVelKorrection = 0;

                    /* this FB is now active */
                    this->ChangeState(FBS_ACTIVE);
                }
            }
        }
        else
        {
            this->ChangeState(FBS_ERROR);
        }
        break;
    }
    case MC_PLCOPEN_STATE_STOPPING:
    {
        /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
        * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
        */
        this->wErrorID = MC_ERR_BUSY_STOPPING;
        this->ChangeState(FBS_ERROR);
        break;
    }
    default:
    {
        this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
        this->ChangeState(FBS_ERROR);
    }
    }
}

void MC_GEAR_IN_T::StateTrans_Active()
{
}


void MC_GEAR_IN_T::DoState_Active()
{
    {
        this->ChangeState(FBS_DONE);

        /* now slave axis is synchronized with master axis */
        pAxis->ePLCOpenState = MC_PLCOPEN_STATE_SYNC_MOTION;

        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: Exe=%d Busy=%d Active=%d InSync=%d CmdAborted=%d Error=%d\n",
            GetName(), this->bExecute, this->bBusy, this->bActive, this->bInGear, this->bCommandAborted, this->bError));
    }
}

MC_T_BOOL MC_GEAR_IN_T::StateMachine()
{
    MC_T_BOOL continueSm = MC_FALSE; // false if state machine should terminate, true to keep running.

    switch (this->fbState)
    {
    case FBS_IDLE:
    {
        if (this->bExecute)
        {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
                this->bError = MC_TRUE;
                continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
                this->bBusy = MC_TRUE;
                continueSm = MC_TRUE;
            }
        }
        break;
    }
    case FBS_BUSY:
    {
        DoState_Busy();
        if (this->fbState == FBS_ACTIVE)
        {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
        }

        if (this->fbState == FBS_ERROR)
        {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
        }
        break;
    }
    case FBS_ACTIVE:
    {
        DoState_Active();
        if (this->fbState == FBS_DONE)
        {
            this->bInGear = MC_TRUE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            continueSm = MC_TRUE;
        }
        else if (this->fbState == FBS_ERROR)
        {
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
        }
        break;
    }
    case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
    case FBS_ERROR:
    case FBS_DONE:
    {
        /* 2.4.1 General rules - Output status
        * Done, Error, ErrorID and CommandAborted are reset with the falling edge
        * of Execute.
        * It must be guaranted that the coresponding outputs are set for at least
        * one cycle...
        */
        if (!this->bExecute)
        {
            this->bInGear = MC_FALSE;
            this->bCommandAborted = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
        }
        break;
    }
    }

    /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
    OsDbgAssert(IsExclusive(this->bBusy, this->bInGear, this->bError, this->bCommandAborted));
    /* ... If Execute is true, one of these outputs has to be true. */
    OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bInGear, this->bError, this->bCommandAborted));

    return continueSm;
}

void MC_GEAR_IN_T::OnCycle()
{
    while (StateMachine());
}


/********************************************************************************/
/** \brief This Function Block disengages the Slave axis from the Master axis (4.6)

*/

void MC_GEAR_OUT_T::StateTrans_Execute()
{
    this->ChangeState(FBS_BUSY);
}

void MC_GEAR_OUT_T::DoState_Busy()
{

    /* Change the state of the slave axis from ‘SynchronizedMotion’ to ‘ContinuousMotion’. Issuing one of this FP in any other state generates an error. */
    switch (pAxis->ePLCOpenState)
    {
        case MC_PLCOPEN_STATE_SYNC_MOTION:
        case MC_PLCOPEN_STATE_ERROR_STOP:
        {
            /* unbind slave from master */
            pAxis->eSetValSource = pAxis->eSetValSourceOrg;
            pAxis->pSyncMasterAxis = EC_NULL;
            pAxis->oGearingData.nRatioNumerator = 0;
            pAxis->oGearingData.dwRatioDenominator = 0;

            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISC_MOTION;
            /* this FB is now active */
            this->ChangeState(FBS_ACTIVE);
            break;
        }
        case MC_PLCOPEN_STATE_STOPPING:
        {
            /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
            * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
            */
            this->wErrorID = MC_ERR_BUSY_STOPPING;
            this->ChangeState(FBS_ERROR);
            break;
        }
        default:
        {
            this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
            this->ChangeState(FBS_ERROR);
        }
    }
}

void MC_GEAR_OUT_T::StateTrans_Active()
{
    MC_T_MOVEMENT*  pMove = &pAxis->oMove;

    if (pMove->lCommandVel == 0) return; // Brake velocity must be set

    if (this->fDeceleration == 0)
    {
        this->wErrorID = MC_ERR_DEC_OUT_OF_RANGE;
        this->ChangeState(FBS_ERROR);
    }

    this->wErrorID = CalcBrakeRamp(pAxis, this->fDeceleration, this->fJerk);
    if (this->wErrorID != MC_NO_ERROR)
    {
        this->ChangeState(FBS_ERROR);
        return;
    }
}

void MC_GEAR_OUT_T::DoState_Active()
{
    MC_T_BOOL bDoneLocal = MC_FALSE;

    if (!((pAxis->ePLCOpenState == MC_PLCOPEN_STATE_DISC_MOTION) ||
        (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STAND_STILL) ||
        (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_HOMING)))
    {
        this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
        this->ChangeState(FBS_ERROR);
        return;
    }

    /* check if done: axis stopped */
    this->wErrorID = MC_CheckTargetPosReached(pAxis, &bDoneLocal);
    if (this->wErrorID != MC_NO_ERROR)
    {
        this->ChangeState(FBS_ERROR);
        return;
    }

    if (bDoneLocal)
    {
        pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
        this->ChangeState(FBS_DONE);

        EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "%s: Exe=%d Busy=%d Active=%d Done=%d Error=%d\n",
            GetName(), this->bExecute, this->bBusy, this->bActive, this->bDone, this->bError));
    }
}


MC_T_BOOL MC_GEAR_OUT_T::StateMachine()
{
    MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

    switch (this->fbState)
    {
    case FBS_IDLE:
    {
        if (this->bExecute)
        {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
                this->bError = MC_TRUE;
                continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
                this->bBusy = MC_TRUE;
                continueSm = MC_TRUE;
            }
        }
        break;
    }
    case FBS_BUSY:
    {
        DoState_Busy();
        if (this->fbState == FBS_ACTIVE)
        {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
        }

        if (this->fbState == FBS_ERROR)
        {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
        }
        break;
    }
    case FBS_ACTIVE:
    {
        DoState_Active();
        if (this->fbState == FBS_DONE)
        {
            this->bDone = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
        }
        else if (this->fbState == FBS_ERROR)
        {
            this->bError = MC_TRUE;
            this->bActive = MC_FALSE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            continueSm = MC_TRUE;
        }
        break;
    }
    case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
    case FBS_ERROR:
    case FBS_DONE:
    {
        /* 2.4.1 General rules - Output status
        * Done, Error, ErrorID and CommandAborted are reset with the falling edge
        * of Execute.
        * It must be guaranted that the coresponding outputs are set for at least
        * one cycle...
        */
        EC_T_DWORD dwCycleCount = 1;

        if (pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
        if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
        {
            this->bDone = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            this->ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
        }
        break;
    }
    }

    /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
    OsDbgAssert(IsExclusive(this->bBusy, this->bDone, this->bError));
    /* ... If Execute is true, one of these outputs has to be true. */
    OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bDone, this->bError));

    return continueSm;
}

void MC_GEAR_OUT_T::OnCycle()
{
    while (StateMachine());
}

#endif /* EC_MOTION_SUPPORT_MC_GEARING */


/*******************************************************************************************************/
MC_T_BOOL AMC_CHECK_TARGETPOS_REACHED_T::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            this->bBusy = MC_TRUE;
            ChangeState(FBS_BUSY);
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_BUSY:
      {
         if (pAxis->oMove.lRemainDist <= 0)
         {
            MC_T_INT lDiff = labs((MC_T_INT)((pAxis->lCommandedPosition/pAxis->lIncFactor) - (pAxis->lActualPosition/pAxis->lIncFactor)));
            if ((EC_T_DWORD)lDiff <= this->dwPositionWindow)
            {
               this->bBusy = MC_FALSE;
               this->bInPosition = MC_TRUE;
               ChangeState(FBS_DONE);
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_DONE:
      {
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bInPosition = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         break;
      }
   default:
      break;
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->InPosition, this->bError));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->InPosition, this->bError));

   return continueSm;
}

/********************************************************************************/
/** \brief Check if target position has been reached

    Check if the actual position has reached the commanded position.
 */
void AMC_CHECK_TARGETPOS_REACHED_T::OnCycle()
{
   while (StateMachine());
}

void AMC_HALT_RECOVERY_T::StateTrans_Execute()
{
   switch (this->recoveryMode)
   {
      case MC_RECOVERY_ABORT_MOVEMENT:
      {
         if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_DISC_MOTION)
         {
            MC_AbortCommand(pAxis, MCFB_ID_IDLE);

            pAxis->oMove.lNextMoveCorrection = 0;
            MC_InitMovement(&pAxis->oMove, MC_FALSE, 0);
            pAxis->oMove.lCommandVel = pAxis->oMove.lCommandAcc = 0;

            MC_DriveSetTargetPosition(pAxis, pAxis->lActualPosition);

            pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
         }
         break;
      }
      case MC_RECOVERY_NO_ACTION:
      default:
         break;
   }

   ChangeState(FBS_DONE);
}

MC_T_BOOL AMC_HALT_RECOVERY_T::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
            }
            else if (this->fbState == FBS_DONE)
            {
               this->bDone = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_DONE:
   case FBS_ERROR:
      {
         EC_T_DWORD dwCycleCount = 1;

         if(pAxis != EC_NULL) dwCycleCount = pAxis->dwCycleCount;
         if ((dwCycleCount - this->dwStateChCycle) > 0 && !this->bExecute)
         {
            this->bDone = MC_FALSE;
            this->bError = MC_FALSE;
            this->wErrorID = MC_NO_ERROR;
            ChangeState(FBS_IDLE);
            continueSm = MC_TRUE;
         }
         break;
      }
   default:
      break;
   }

   /* 2.4.1 General rules - Output exclusivity: The outputs Busy, Done, Error and CommandAborted are mutually
    * exclusive.
    */
   OsDbgAssert(IsExclusive(this->bBusy, this->bDone, this->bError));
   /* ... If Execute is true, one of these outputs has to be true. */
   OsDbgAssert(!this->bExecute || IsAnyOn(this->bBusy, this->bDone, this->bError));

   return continueSm;
}

/********************************************************************************/
/** \brief Recover from motion halt (e.g. desired torque reached)
 */
void AMC_HALT_RECOVERY_T::OnCycle()
{
   while (StateMachine());
}

/********************************************************************************/
/** \brief Calculate move times and segment distances

*/

EC_T_WORD   MC_CalcMoveProfile
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_REAL              fDistanceParam  /* IN: Distance for the motion */
    ,MC_T_REAL              fVelocity       /* IN: Value of the maximum velocity (always positive, not necessarily reached). */
    ,MC_T_REAL              fAcceleration   /* IN: Value of the acceleration (always positive, increasing energy of the motor). */
    ,MC_T_REAL              fDeceleration   /* IN: Value of the deceleration (always positive, decreasing energy of the motor). */
    ,MC_T_REAL              fJerk           /* IN: Value of the Jerk (always positive). */
    ,MC_T_MOVEMENT*         pMove)
{
    return MC_CalcMoveProfileBuffered(pAxis, fDistanceParam, fVelocity, fAcceleration, fDeceleration, fJerk,
        MC_ABORTING, MC_ABORTING, 0, 0, pMove);
}



EC_T_WORD   MC_CalcMoveProfileBuffered
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_REAL              fDistanceParam  /* IN: Distance for the motion */
    ,MC_T_REAL              fVelocity       /* IN: Value of the maximum velocity (always positive, not necessarily reached). */
    ,MC_T_REAL              fAcceleration   /* IN: Value of the acceleration (always positive, increasing energy of the motor). */
    ,MC_T_REAL              fDeceleration   /* IN: Value of the deceleration (always positive, decreasing energy of the motor). */
    ,MC_T_REAL              fJerk           /* IN: Value of the Jerk (always positive). */
    ,EC_T_DWORD             dwBufferMode    /* IN: Defines the chronological sequence */
    ,EC_T_DWORD             dwBufferModeNext /* IN: Defines the chronological sequence for next FB */
    ,MC_T_REAL              fVelocityPrev   /* IN: Velocity of FB before: Only if BufferMode == MC_BLENDING_ */
    ,MC_T_REAL              fVelocityNext   /* IN: Velocity of FB after:  Only if BufferMode == MC_BLENDING_ */
    ,MC_T_MOVEMENT*         pMove)
{
    EC_T_WORD       wRes = MC_NO_ERROR;
    MC_T_POS_CALC   lDistance = 0;
    MC_T_REAL       fStartVel = fVelocityPrev;
    MC_T_REAL       fEndVel = fVelocityNext;

    if((pAxis == EC_NULL) || (pMove == EC_NULL))
    {
        return MC_ERR_INVALID_PARAMETER;
    }

    /* check resolution */
    if(pAxis->dwIncPerMM == 0)
    {
        return MC_ERR_INVALID_RESOLUTION;
    }

    /* check cycle time */
    if(pAxis->dwCycleTime == 0)
    {
        return MC_ERR_INVALID_CYCLETIME;
    }

    /* calculate start velocity for blending modes */
    switch (dwBufferMode)
    {
    case MC_BLENDING_LOW:
        fStartVel = EC_MIN(fVelocityPrev, fVelocity);
        break;
    case MC_BLENDING_PREVIOUS:
    case MC_BLENDING_PREVIOUS_STOP:
        fStartVel = fVelocityPrev;
        break;
    case MC_BLENDING_NEXT:
    case MC_BLENDING_NEXT_STOP:
        fStartVel = fVelocity;
        break;
    case MC_BLENDING_HIGH:
    case MC_BLENDING_HIGH_STOP:
        fStartVel = EC_MAX(fVelocityPrev, fVelocity);
        break;
    default:
        break;
    }

    /* calculate end velocity for blending modes */
    switch (dwBufferModeNext)
    {
    case MC_BLENDING_LOW:
        fEndVel = EC_MIN(fVelocityNext, fVelocity);
        break;
    case MC_BLENDING_PREVIOUS:
    case MC_BLENDING_PREVIOUS_STOP:
        fEndVel = fVelocity;
        break;
    case MC_BLENDING_NEXT:
    case MC_BLENDING_NEXT_STOP:
        fEndVel = fVelocityNext;
        break;
    case MC_BLENDING_HIGH:
    case MC_BLENDING_HIGH_STOP:
        fEndVel = EC_MAX(fVelocityNext, fVelocity);
        break;
    default:
        break;
    }

    /* do have to calculate the conversion factors ? */
    if(pAxis->dwCalcIncPerMM == 0)
    {
        pAxis->dwCalcIncPerMM = EC_MAX(1, pAxis->dwIncPerMM*pAxis->lIncFactor);
        pAxis->fVelToInc  = (MC_T_REAL)((MC_T_POS_CALC)pAxis->dwCalcIncPerMM * pAxis->dwCycleTime/1E6);
        pAxis->fAccToInc  = (MC_T_REAL)(pAxis->fVelToInc  * pAxis->dwCycleTime/1E6);
        pAxis->fJerkToInc = (MC_T_REAL)(pAxis->fAccToInc  * pAxis->dwCycleTime/1E6);
    }

    if(fDistanceParam >= 0)
    {
        lDistance = (MC_T_POS_CALC)(fDistanceParam * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
    }
    else
    {
        lDistance = (MC_T_POS_CALC)(-fDistanceParam * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
    }

    OsMemset(pMove, 0, sizeof(MC_T_MOVEMENT));
    pMove->lCommandVel = (MC_T_VELACC)(fStartVel * pAxis->fVelToInc);

    /* calculate movement and setup parameters */
    if(fJerk != 0)
    {
        wRes = MC_TrajSetupJerkMove(pAxis, pMove, MC_TRUE, MC_FALSE, lDistance, fVelocity, fEndVel, fAcceleration, fDeceleration, fJerk);
    }
    else
    {
        wRes = MC_TrajSetupTrapezeMove(pAxis, pMove, MC_TRUE, MC_FALSE, lDistance, fVelocity, fEndVel, fAcceleration, fDeceleration);
    }

    return wRes;
}


/********************************************************************************/
/** \brief Calculate time until a certain position is reached

  The function MC_CalcMoveProfile() has to be called prior calling this function.

  Important hint: Data in *pMove will be modified by this function!
                  Therefore do not use pAxis->oMove.
                  To get pTriggerTime based on the current move, copy pAxis->oMove
                  into a local data buffer before calling this function.
*/
EC_T_WORD MC_CalcMoveTimeAtPos
    (MC_T_AXIS_REF*         pAxis           /* IN: Axis parameters */
    ,MC_T_MOVEMENT*         pMove           /* IN: Move parameters */
    ,MC_T_REAL              fTriggerDist    /* IN: Distance from start */
    ,EC_T_INT64*            pTriggerTime)   /* OUT: Trigger time */
{
    EC_T_WORD       wRes = MC_NO_ERROR;
    MC_T_POS_CALC   lDistance = 0;

    if((pAxis == EC_NULL) || (pMove == EC_NULL) || (pTriggerTime == EC_NULL))
    {
        return MC_ERR_INVALID_PARAMETER;
    }

    /* check resolution */
    if(pAxis->dwIncPerMM == 0)
    {
        return MC_ERR_INVALID_RESOLUTION;
    }

    /* check cycle time */
    if(pAxis->dwCycleTime == 0)
    {
        return MC_ERR_INVALID_CYCLETIME;
    }

    if(fTriggerDist >= 0)
    {
        lDistance = (MC_T_POS_CALC)(fTriggerDist * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
    }
    else
    {
        lDistance = (MC_T_POS_CALC)(-fTriggerDist * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
    }

    /* only accending distances are allowed */
    if(pMove->lDoneDist > lDistance)
    {
        wRes = MC_ERR_INVALID_PARAMETER;
    }
    /* trigerpoint out-of-range ? */
    else if(lDistance > pMove->lTotalDist)
    {
        wRes = MC_ERR_INVALID_PARAMETER;
    }
    else
    {
        while((pMove->lDoneDist < lDistance) && (pMove->lRemainDist != 0))
        {
            /* this call updates dwDoneCycles etc. */
            MC_TrajNextSetPoint(pMove);
            pMove->lRemainDist = pMove->lRemainDist - pMove->lTargetPosStep;
        }

        *pTriggerTime = pMove->dwDoneCycles * pAxis->dwCycleTime;
    }
    return wRes;
}


/********************************************************************************/
/** \brief Initialize MC_T_MOVEMENT for new move
*
* \return N/A
*/
static EC_T_VOID   MC_InitMovement
    (MC_T_MOVEMENT*         pMove
    ,MC_T_BOOL              bEndless
    ,MC_T_POS_CALC          lDistance)
{
    if(pMove->lNextMoveCorrection != 0)
    {
        lDistance = lDistance + pMove->lNextMoveCorrection;     /* remainder previous move */
        pMove->lNextMoveCorrection = 0;
    }

    pMove->lTotalDist = pMove->lRemainDist = lDistance;
    pMove->lDoneDist  = pMove->dwDoneCycles = 0;
    pMove->lRemainStep = 0;
    pMove->bRemainStepCalc = MC_FALSE;
    pMove->lSegDist1to3  = 0;
    pMove->lSegDist5to7  = 0;
    pMove->bEndless = bEndless;
    pMove->lEndVel = pMove->lConstantVel = 0;
    pMove->nRampIdx = 0;
    pMove->lVelRemainder[0] = pMove->lVelRemainder[1] = 0;
    pMove->lVelReduce[0] = pMove->lVelReduce[1] = 0;
    pMove->lRampAccMax[0] = pMove->lRampAccMax[1] = 0;
    pMove->lRamp2AccCorrection = 0;

    OsMemset(pMove->lSegDist, 0, sizeof(pMove->lSegDist));
}

static MC_T_VELACC   MC_MoveSetupCheckMaxAcc
    (MC_T_VELACC            lJerk
    ,MC_T_VELACC            lCurrAcc                /* negative and positive values */
    ,MC_T_VELACC            lMaxAcc
    ,MC_T_VELACC            lVelDiff)
{
    MC_T_VELACC           lAccUpVel, lAccDownVel, lRemainLocal;
    MC_T_VELACC           lCurrVelDiff;
    MC_T_VELACC           lReduceStep;
    MC_T_VELACC           lSteps, lDiffAcc;

    if(lJerk == 0)
        goto Exit;

    if(lVelDiff == 0)
        goto Exit;

    /* reduce acc if to big for lVelDiff */
    for(; lMaxAcc > 1; )
    {
        lAccUpVel = lAccDownVel = 0;

        lDiffAcc = lMaxAcc - lCurrAcc;
        if(lDiffAcc > 0)
        {
            lSteps = lDiffAcc/lJerk;
            lAccUpVel = (((lSteps+1)*lSteps)*lJerk)/2;
            lAccUpVel +=  lSteps*lCurrAcc;
            if(lDiffAcc%lJerk!=0) lAccUpVel += lMaxAcc;     // segment with Acc equal MaxAcc
        }

        lSteps = lMaxAcc/lJerk;
        lRemainLocal = lMaxAcc % lJerk;
        lAccDownVel = (((lSteps-1)*lSteps)*lJerk)/2;
        if(lRemainLocal) lAccDownVel += (lSteps*lRemainLocal);

        lCurrVelDiff = lVelDiff - lAccUpVel - lAccDownVel;
        if( lCurrVelDiff > 0)
        {
            break;
        }
        else
        {
            EC_T_INT        lReduceFactor;

            lCurrVelDiff = EC_MIN(-lCurrVelDiff, LONG_MAX/100);
            lReduceFactor = (EC_T_INT)(lCurrVelDiff*100/lVelDiff) + 1;
            lReduceFactor = EC_MIN(lReduceFactor, 25);
            lReduceStep    = EC_MAX((MC_T_VELACC)((MC_T_REAL)lReduceFactor/100.0 * lMaxAcc), 1);
            lMaxAcc = EC_MAX(1, lMaxAcc-lReduceStep);
        }
    }
Exit:
    return lMaxAcc;
}


static MC_T_VELACC   MC_MoveSetupCheckMaxDec
    (MC_T_VELACC            lJerk
    ,MC_T_VELACC            lCurrAcc                /* negative and positive values */
    ,MC_T_VELACC            lMaxDec
    ,MC_T_VELACC            lVelDiff)
{
    MC_T_VELACC           lDecUpVel, lDecDownVel, lRemainLocal;
    MC_T_VELACC           lCurrVelDiff;
    MC_T_VELACC           lReduceStep;
    MC_T_VELACC           lSteps, lDiffAcc;

    if(lJerk == 0)
        goto Exit;

    if(lVelDiff == 0)
        goto Exit;

    lCurrAcc = -lCurrAcc;        /* positive lMaxDec values therefore we have to change sign here */
    for(; lMaxDec > 1; )
    {
        lDecUpVel = lDecDownVel = 0;

        /* vel sum for increasing dec */
        lDiffAcc = lMaxDec - lCurrAcc;
        if(lDiffAcc > 0)
        {
            lSteps = lDiffAcc/lJerk;
            lDecUpVel = (((lSteps+1)*lSteps)*lJerk)/2;
            lDecUpVel +=  lSteps*lCurrAcc;
            if(lDiffAcc%lJerk != 0)
                lDecUpVel += lMaxDec;
        }

        /* vel sum for decreasing dec */
        lSteps  = lMaxDec/lJerk;
        lRemainLocal = lMaxDec%lJerk;
        lDecDownVel = (((lSteps-1)*lSteps)*lJerk)/2;
        if(lRemainLocal) lDecDownVel += (lSteps*lRemainLocal);

        lCurrVelDiff = lVelDiff - lDecUpVel - lDecDownVel;
        if( lCurrVelDiff > 0)
        {
            break;
        }
        else
        {
            EC_T_INT        lReduceFactor;

            lCurrVelDiff = EC_MIN(-lCurrVelDiff, LONG_MAX/100);
            lReduceFactor = (EC_T_INT)(lCurrVelDiff*100/lVelDiff) + 1;
            lReduceFactor = EC_MIN(lReduceFactor, 25);
            lReduceStep    = EC_MAX((MC_T_VELACC)((MC_T_REAL)lReduceFactor/100.0*lMaxDec), 1);
            lMaxDec = EC_MAX(1, lMaxDec-lReduceStep);
        }
    }

Exit:
    return lMaxDec;
}


/********************************************************************************/
/** \brief Calculate jerk limited acc ramp
*
* \return: error code
*/
static EC_T_WORD   MC_TrajJerkCalcAccRamp
    (MC_T_MOVEMENT*         pMove
    ,EC_T_INT               nRampIdx        /* 0 or 1 for ramp 1 or 2 */
    ,MC_T_VELACC            lCurrAcc        /* negative and positive values */
    ,MC_T_VELACC            lCurrVel
    ,MC_T_VELACC            lTargetVel
    ,MC_T_VELACC            lMaxAcc
    ,MC_T_VELACC            *plEndAcc)
{
    EC_T_WORD       wErrorID = MC_NO_ERROR;
    MC_T_VELACC     lStartVel  = lCurrVel;
    MC_T_VELACC     lNextVel = 0;
    MC_T_VELACC     lTargetAcc = 0, lSteps;
    EC_T_INT        nSegNum = 1;
    MC_T_VELACC     lVelRemainder = 0;
    MC_T_VELACC     lAccDownVel = 0;
    MC_T_VELACC     lVelDiff;
    EC_T_INT        nSegBaseIndex = (nRampIdx == 0) ? 0 : 4;
    MC_T_BOOL       bRemainVelCalc = EC_FALSE;

    pMove->lSegDist[nSegBaseIndex]   = pMove->lSegCycles[nSegBaseIndex]   = 0;
    pMove->lSegDist[nSegBaseIndex+1] = pMove->lSegCycles[nSegBaseIndex+1] = 0;
    pMove->lSegDist[nSegBaseIndex+2] = pMove->lSegCycles[nSegBaseIndex+2] = 0;
    pMove->lVelRemainder[nRampIdx] = 0;

    lVelDiff = lTargetVel-lCurrVel;
    OsDbgAssert(lMaxAcc > 0);
    OsDbgAssert(lVelDiff >= 0);

    if(lVelDiff == 0)  /* in case we have no vel difference exit here */
        goto Exit;

    if(pMove->lJerk != 0)
    {
        lMaxAcc = MC_MoveSetupCheckMaxAcc(pMove->lJerk, lCurrAcc, lMaxAcc, lVelDiff);

        /* calc vel diff during dec of acc */
        lSteps = lMaxAcc/pMove->lJerk;
        lAccDownVel = (((lSteps-1)*lSteps)*pMove->lJerk)/2;
        lAccDownVel +=  lSteps*(lMaxAcc%pMove->lJerk);
        pMove->lVelReduce[nRampIdx] = lAccDownVel;
    }
    else
    {
        lMaxAcc = EC_MIN(lMaxAcc, lVelDiff);
    }

    pMove->lRampAccMax[nRampIdx] = lMaxAcc;

    for( lCurrVel = lStartVel; (lCurrVel != lTargetVel) && (wErrorID == MC_NO_ERROR); )
    {
        /* has acc to be increased or decreased ? */
        if(lCurrVel <= lTargetVel - pMove->lVelReduce[nRampIdx] - lCurrAcc)
        {
            lTargetAcc = EC_MIN(lTargetVel - lCurrVel, lMaxAcc);         /* calc target acc */
            if(lCurrAcc < lTargetAcc)
            {
                /* increase acc */
                if(pMove->lJerk != 0)
                    lCurrAcc = EC_MIN(lCurrAcc + pMove->lJerk, lTargetAcc);
                else
                    lCurrAcc = lTargetAcc;
                lNextVel = EC_MIN(lCurrVel + lCurrAcc, lTargetVel);
                lNextVel = EC_MAX(lNextVel, 0);     // no negative velocity
                if(lCurrAcc == lMaxAcc) nSegNum = 2; else nSegNum = 1;
            }
            else if(lCurrAcc == lTargetAcc)         /* constant acc  */
            {
                OsDbgAssert(lCurrAcc != 0);
                lNextVel = EC_MIN(lCurrVel + lCurrAcc, lTargetVel);
                if(lCurrAcc == lMaxAcc) nSegNum = 2; else nSegNum = 3;
            }
            else
            {
                /* decrease acc */
                if(pMove->lJerk != 0)
                    lCurrAcc = EC_MAX(lCurrAcc - pMove->lJerk, lTargetAcc);
                else
                    lCurrAcc = lTargetAcc;
                lNextVel = EC_MIN(lCurrVel + lCurrAcc, lTargetVel);
                if(lCurrAcc == lMaxAcc) nSegNum = 2; else nSegNum = 3;
            }
        }
        else
        {
            /* decrease acc */
            if(!bRemainVelCalc && (lCurrAcc == lMaxAcc))
            {
                /* calculate vel remainder package */
                bRemainVelCalc = MC_TRUE;
                pMove->lVelRemainder[nRampIdx] = lVelRemainder = EC_MAX((lTargetVel - lCurrVel - pMove->lVelReduce[nRampIdx]), 0);
                OsDbgAssert(lVelRemainder < lCurrAcc);
            }

            MC_T_VELACC lNextAcc = 0;

            if(pMove->lJerk != 0)
                lNextAcc = lCurrAcc - pMove->lJerk;
            if(lNextAcc <= 0)
                lNextAcc = EC_MIN(lTargetVel - lCurrVel, lMaxAcc);

            // do we have to insert the VelRemainder ?
            if((lVelRemainder != 0) && (lNextAcc <= lVelRemainder))
            {
                lNextVel = EC_MIN(lCurrVel + lVelRemainder, lTargetVel);
                if(lNextVel == lTargetVel)
                    lCurrAcc = lVelRemainder;
                lVelRemainder = 0;
            }
            else
            {
                lCurrAcc =  lNextAcc;
                lNextVel = EC_MIN(lCurrVel + lCurrAcc, lTargetVel);
            }
            if(lNextVel - lCurrVel == lMaxAcc) nSegNum = 2; else nSegNum = 3;
        }

        if(nSegNum == 2)
        {
            MC_T_VELACC         lDiff, lRemainLocal;
            MC_T_POS_CALC       nSteps;
            MC_T_POS_CALC       llRamp = 0;

            lNextVel = EC_MAX(lNextVel, lTargetVel-pMove->lVelReduce[nRampIdx]);
            lDiff = lNextVel - lCurrVel;
            if(lDiff != 0)
            {
                nSteps  = lDiff / lMaxAcc;
                lRemainLocal = lDiff % lMaxAcc;
                lNextVel -= lRemainLocal;
                pMove->lSegCycles[nSegBaseIndex+nSegNum-1] = (EC_T_INT)nSteps;

                llRamp = ((lCurrVel) * (nSteps));
                llRamp += ((((nSteps)+1)*nSteps)*lMaxAcc)/2;
            }
            pMove->lSegDist[nSegBaseIndex+nSegNum-1] = llRamp;
        }
        else if((nSegNum >= 1) && (nSegNum <= 3))
        {
            pMove->lSegDist[nSegBaseIndex+nSegNum-1] += labs(lNextVel);
            pMove->lSegCycles[nSegBaseIndex+nSegNum-1]++;
            /* check for overflow */
            if(pMove->lSegDist[nSegBaseIndex+nSegNum-1] < 0)
            {
                wErrorID = MC_ERR_OVERFLOW;
            }
        }
        lCurrVel = lNextVel;
    }

    OsDbgAssert(lVelRemainder == 0);
Exit:
    if(plEndAcc) *plEndAcc = lCurrAcc;
    return wErrorID;
}


/********************************************************************************/
/** \brief Calculate jerk limited dec ramp
*
* \return: error code
*/
static EC_T_WORD   MC_TrajJerkCalcDecRamp
    (MC_T_MOVEMENT*         pMove
    ,EC_T_INT               nRampIdx        /* 0 or 1 for ramp 1 or 2 */
    ,MC_T_VELACC            lCurrAcc        /* negative and positive values */
    ,MC_T_VELACC            lCurrVel
    ,MC_T_VELACC            lTargetVel
    ,MC_T_VELACC            lMaxDec
    ,MC_T_VELACC            *plEndAcc)
{
    EC_T_WORD       wErrorID = MC_NO_ERROR;
    MC_T_VELACC     lStartVel = lCurrVel;
    MC_T_VELACC     lNextVel = 0;
    MC_T_VELACC     lTargetAcc = 0;
    MC_T_VELACC     lSteps, lRemainLocal;
    EC_T_INT        nSegNum = 1;
    MC_T_VELACC     lVelRemainder = 0;
    MC_T_VELACC     lDecDownVel = 0;
    MC_T_VELACC     lVelDiff;
    EC_T_INT        nSegBaseIndex = (nRampIdx == 0) ? 0 : 4;
    MC_T_BOOL       bRemainVelCalc = EC_FALSE;

    pMove->lSegDist[nSegBaseIndex]   = pMove->lSegCycles[nSegBaseIndex] = 0;
    pMove->lSegDist[nSegBaseIndex+1] = pMove->lSegCycles[nSegBaseIndex+1] = 0;
    pMove->lSegDist[nSegBaseIndex+2] = pMove->lSegCycles[nSegBaseIndex+2] = 0;
    pMove->lVelRemainder[nRampIdx] = 0;

    lVelDiff = lTargetVel - lCurrVel;
    OsDbgAssert(lVelDiff <= 0);
    if(lVelDiff == 0)          /* in case we have no vel difference exit here */
        goto Exit;

    if(pMove->lJerk != 0)
    {
        lMaxDec = -MC_MoveSetupCheckMaxDec(pMove->lJerk, lCurrAcc, -lMaxDec, -lVelDiff);

        /* calc vel diff during dec of dec */
        lSteps  = -lMaxDec / pMove->lJerk;
        lRemainLocal = -lMaxDec % pMove->lJerk;
        lDecDownVel = -(((lSteps-1)*lSteps)*pMove->lJerk)/2;
        if(lRemainLocal)
            lDecDownVel -= lSteps*lRemainLocal;

        pMove->lVelReduce[nRampIdx] = lDecDownVel;
    }
    else
    {
        lMaxDec = EC_MAX(lMaxDec, lVelDiff);
    }

    pMove->lRampAccMax[nRampIdx] = lMaxDec;

    for(lCurrVel = lStartVel; (lCurrVel != lTargetVel) && (wErrorID == MC_NO_ERROR); )
    {
        nSegNum = 0;
        /* calc target dec */
        if(lCurrVel >= lTargetVel - pMove->lVelReduce[nRampIdx] - lCurrAcc)
        {
            lTargetAcc = EC_MAX(lTargetVel - lCurrVel, lMaxDec);
            if(lCurrAcc > lTargetAcc)
            {
                if(pMove->lJerk != 0)
                    lCurrAcc = EC_MAX(lCurrAcc - pMove->lJerk, lTargetAcc);
                else
                    lCurrAcc = lTargetAcc;
                lNextVel = EC_MAX(lCurrVel + lCurrAcc, lTargetVel);
                if(lNextVel - lCurrVel == lMaxDec)  nSegNum = 2;
                else                                 nSegNum = 1;
            }
            else if(lCurrAcc == lTargetAcc)
            {
                /* constant acc */
                OsDbgAssert(lCurrAcc != 0);
                lNextVel = EC_MAX(lCurrVel + lCurrAcc, lTargetVel);
                if(lNextVel - lCurrVel == lMaxDec)  nSegNum = 2;
                else                                 nSegNum = 3;
            }
            else
            {
                if(pMove->lJerk != 0)
                    lCurrAcc = EC_MIN(lCurrAcc + pMove->lJerk, lTargetAcc);
                else
                    lCurrAcc = lTargetAcc;

                lNextVel = EC_MAX(lCurrVel + lCurrAcc, lTargetVel);
                if(lNextVel - lCurrVel == lMaxDec) nSegNum = 2;
                else                                nSegNum = 1;
            }
        }
        else
        {
            /* decrease dec */
            if(!bRemainVelCalc && (lCurrAcc == lMaxDec))
            {
                /* calculate vel remainder package */
                bRemainVelCalc = MC_TRUE;
                if(lTargetVel - lCurrVel < -pMove->lJerk)
                    pMove->lVelRemainder[nRampIdx] = lVelRemainder = (lTargetVel - lCurrVel - pMove->lVelReduce[nRampIdx]);
                OsDbgAssert(lVelRemainder > lCurrAcc);
                OsDbgAssert(lVelRemainder <= 0);
            }

            MC_T_VELACC lNextAcc = 0;

            if(pMove->lJerk != 0)
            {
                lNextAcc = EC_MIN(lCurrAcc + pMove->lJerk, 0);
            }
            if(lNextAcc >= 0)
                lNextAcc = EC_MAX(lTargetVel - lCurrVel, lMaxDec);

            // do we have to insert the VelRemainder ?
            if((lVelRemainder != 0) && (-lNextAcc <= -lVelRemainder))
            {
                lNextVel = EC_MAX(lCurrVel + lVelRemainder, lTargetVel);
                if(lNextVel == lTargetVel)
                    lCurrAcc = lVelRemainder;
                lVelRemainder = 0;
            }
            else
            {
                lCurrAcc = lNextAcc;
                lNextVel = EC_MAX(lCurrVel + lCurrAcc, lTargetVel);
            }
            if(lNextVel - lCurrVel == lMaxDec) nSegNum = 2; else nSegNum = 3;
        }

        if(nSegNum == 2)
        {
            MC_T_VELACC         lDiff;
            MC_T_POS_CALC       nSteps;
            MC_T_POS_CALC       llRamp = 0;
            MC_T_VELACC         lTempAcc = -lMaxDec;

            lNextVel = EC_MIN(lNextVel, lTargetVel-pMove->lVelReduce[nRampIdx]);
            lDiff = lCurrVel - lNextVel;
            if(lDiff > 0)
            {
                nSteps  = lDiff / lTempAcc;
                lRemainLocal = lDiff % lTempAcc;
                lNextVel += lRemainLocal;
                pMove->lSegCycles[nSegBaseIndex+nSegNum-1] = (EC_T_INT)nSteps;

                llRamp = ((lCurrVel) * (nSteps));
                llRamp -= ((((nSteps)+1)*nSteps)*lTempAcc)/2;
            }
            pMove->lSegDist[nSegBaseIndex+nSegNum-1] = llRamp;
        }
        else if((nSegNum >= 1) && (nSegNum <= 3))
        {
            pMove->lSegDist[nSegBaseIndex+nSegNum-1] += labs(lNextVel);
            pMove->lSegCycles[nSegBaseIndex+nSegNum-1]++;
            /* check for overrun */
            if(pMove->lSegDist[nSegBaseIndex+nSegNum-1] < 0)
            {
                wErrorID = MC_ERR_OVERFLOW;
            }
        }
        lCurrVel = lNextVel;
    }

    OsDbgAssert(lVelRemainder == 0);

Exit:
    if(plEndAcc) *plEndAcc = lCurrAcc;
    return wErrorID;
}


/********************************************************************************/
/** \brief Calc and check ramp 1
*
* \return: error code
*/
static EC_T_WORD   MC_TrajCheckRamp1
    (MC_T_MOVEMENT*         pMove
    ,MC_T_POS_CALC          lDistance
    ,EC_T_WORD              wErrorCode)
{
    EC_T_WORD       wErrorID = MC_NO_ERROR;

    if(pMove->bEndless) goto Exit;

    if((pMove->lSegDist[0] > lDistance) || (pMove->lSegDist[1] > lDistance) || (pMove->lSegDist[2] > lDistance) || (pMove->lSegDist1to3 > lDistance))
    {
        wErrorID = wErrorCode;
    }
Exit:
    return wErrorID;
}

/********************************************************************************/
/** \brief Calc and check ramp 2
*
* \return: error code
*/
static EC_T_WORD   MC_TrajCheckRamp2
    (MC_T_MOVEMENT*         pMove
    ,MC_T_POS_CALC             lDistance
    ,EC_T_WORD              wErrorCode)
{
    EC_T_WORD       wErrorID = MC_NO_ERROR;

    if(pMove->bEndless) goto Exit;

    if((pMove->lSegDist[4] > lDistance) || (pMove->lSegDist[5] > lDistance) || (pMove->lSegDist[6] > lDistance) || (pMove->lSegDist5to7 > lDistance))
    {
        wErrorID = wErrorCode;
    }
Exit:
    return wErrorID;
}

/*
 * Fix for TI Starterware compiler (NEON issue).
 * It issues an internal compiler error if 3 64bit values are summed in place.
 */
MC_T_POS_CALC Sum3(MC_T_POS_CALC* plSegDist)
{
    return plSegDist[0] + plSegDist[1] + plSegDist[2];
}

/********************************************************************************/
/** \brief Preparation for new trapeze movement
*
* Example 1 : MaxAcc = 3, MaxDec = 3; MaxVel = 15, Dist = 90
*
* Vel =  3,  6,  9,  12, 15,  15, 12, 9, 6, 3
* Seg   [2----------------], [4], [6---------]
*
* Example 2 : MaxAcc = 3, MaxDec = 3; MaxVel = 14, Dist = 88
*
* Vel =  3,  6,  9,  12, 14,  14,  12, 9, 6, 3
* Seg   [2------------], [3], [4],[5], [6-----]
*
* \return N/A
*/
static EC_T_WORD   MC_TrajSetupTrapezeMove
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_MOVEMENT*         pMove
    ,MC_T_BOOL              bNewMove            /* if MC_TRUE, initiate a new movement */
    ,MC_T_BOOL              bEndless
    ,MC_T_POS_CALC          lDistance
    ,MC_T_REAL              fMaxVelocity
    ,MC_T_REAL              fEndVelocity
    ,MC_T_REAL              fAcceleration
    ,MC_T_REAL              fDeceleration)
{
    EC_T_WORD           wErrorID = MC_NO_ERROR;
    MC_T_POS_CALC       lNewDistance;
    MC_T_BOOL           bMoveParaChanged = MC_FALSE;
    MC_T_POS_CALC       lTotalDistSum = 0;
    MC_T_BOOL           bSuccess = MC_FALSE;
    MC_T_VELACC         lVelReduce;
    MC_T_REAL           fReduceFactor;
    MC_T_REAL           fVelAvg;        /* average velocity */
    MC_T_VELACC         lAccRamp1 = 0, lOrgAccRamp1 = 0;
    MC_T_INT            nRecalcRampTwoState = 0;
    MC_T_BOOL           bVariableEndVel = MC_FALSE;

    if(bNewMove)
    {
        MC_InitMovement(pMove, bEndless, lDistance);
        lDistance = pMove->lTotalDist;

        /* due to integer arithmetic later we have to round the float values to get better results */
        fMaxVelocity = EC_MAX(fMaxVelocity, 1/pAxis->fVelToInc);
        pMove->lConstantVel = EC_MAX((MC_T_VELACC)(fMaxVelocity * pAxis->fVelToInc), 1);
        if(lDistance != 0) pMove->lConstantVel = (MC_T_VELACC)EC_MIN(pMove->lConstantVel, lDistance);

        pMove->lMaxAcc = (MC_T_VELACC)(fAcceleration * pAxis->fAccToInc);
        if(lDistance != 0) pMove->lMaxAcc = (MC_T_VELACC)EC_MIN(pMove->lMaxAcc, lDistance/2);
        pMove->lMaxAcc = EC_MIN(pMove->lMaxAcc, pMove->lConstantVel);
        pMove->lMaxAcc = EC_MAX(pMove->lMaxAcc, 1);
        fAcceleration  = pMove->lMaxAcc / pAxis->fAccToInc;

        pMove->lMaxDec = (MC_T_VELACC)(fDeceleration * pAxis->fAccToInc);
        if(lDistance != 0) pMove->lMaxDec = (MC_T_VELACC)EC_MIN(pMove->lMaxDec, lDistance/2);
        pMove->lMaxDec = EC_MIN(pMove->lMaxDec, pMove->lConstantVel);
        pMove->lMaxDec = EC_MAX(pMove->lMaxDec, 1);
        fDeceleration  = pMove->lMaxDec / pAxis->fAccToInc;

        pMove->lJerk        = 0;

        pMove->lEndVel    =  EC_MAX((MC_T_VELACC)(fEndVelocity * pAxis->fVelToInc), 0);

        /* we have to save the initial values */
        pMove->fOrgMaxVelocity   = fMaxVelocity;
        pMove->fOrgEndVelocity   = fEndVelocity;
        pMove->fOrgAcceleration  = fAcceleration;
        pMove->fOrgDeceleration  = fDeceleration;
        pMove->fOrgJerk          = 0;

        if(!bEndless && (lDistance == 0))
        {
            pMove->lConstantVel = 0;
            return wErrorID;
        }
    }
    else
    {
        /* detect if input parameters are changed (Distance, Velocity, Acceleration, Deceleration */
        if(pMove->lTotalDist != lDistance)
        {
            /* check if new distance is acceptable */
            lNewDistance = (lDistance - pMove->lTotalDist) + pMove->lRemainDist;

            if(lNewDistance < pMove->lSegDist5to7)    /* if stopping is in progress, new distance is not accepted */
            {
                /* Error */
                wErrorID = MC_ERR_CONTUPDATE_DIST_CHANGED;
                EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_TrajSetupTrapezeMove(): Distance not possible because smaller than breakramp. %d < %d", lNewDistance, pMove->lSegDist5to7));
            }
            else
            {
                pMove->lRemainDist += (lDistance - pMove->lTotalDist);
                pMove->lTotalDist = lDistance;
                bMoveParaChanged = MC_TRUE;
            }
        }

        lDistance = pMove->lRemainDist;

        if(pMove->fOrgMaxVelocity != fMaxVelocity)        /* max velocity changed ? */
        {
            pMove->fOrgMaxVelocity = fMaxVelocity;
            bMoveParaChanged = MC_TRUE;
        }

        if(pMove->fOrgEndVelocity != fEndVelocity)        /* end velocity changed ? */
        {
            pMove->lEndVel      =  (MC_T_VELACC)(fEndVelocity * pAxis->fVelToInc);
            pMove->lEndVel      =  EC_MAX(pMove->lEndVel, 0);
            pMove->fOrgEndVelocity = fEndVelocity;
            bMoveParaChanged = MC_TRUE;
            bVariableEndVel = MC_TRUE;          /* we can change the "end velocity" already calculated in the previous calc. */
        }

        if(pMove->fOrgAcceleration != fAcceleration)
        {
            pMove->lMaxAcc      =  (MC_T_VELACC)(fAcceleration * pAxis->fAccToInc);
            pMove->lMaxAcc      =  EC_MAX(pMove->lMaxAcc, 1);
            pMove->fOrgAcceleration  = fAcceleration;
            bMoveParaChanged = MC_TRUE;
        }

        if(pMove->fOrgDeceleration != fDeceleration)
        {
            pMove->lMaxDec      =  (MC_T_VELACC)(fDeceleration * pAxis->fAccToInc);
            pMove->lMaxDec      =  EC_MAX(pMove->lMaxDec, 1);
            pMove->fOrgDeceleration  = fDeceleration;
            bMoveParaChanged = MC_TRUE;
        }

        if(!bMoveParaChanged)
            return wErrorID;

        pMove->lConstantVel      =  (MC_T_VELACC)(fMaxVelocity * pAxis->fVelToInc);
        pMove->lConstantVel      =  EC_MAX(pMove->lConstantVel, 1);
        pMove->bRemainStepCalc = MC_FALSE;
    }

    pMove->lStartVel = pMove->lCommandVel;
    pMove->lStartAcc = pMove->lCommandAcc;
    lAccRamp1 = pMove->lStartAcc;

    /* check if move is in general possible */
    {
        MC_T_POS_CALC   lRamp;

        if(pMove->lStartVel < pMove->lEndVel)
        {
            /* acc up to ConstantVel */
            if(pMove->lRampAccMax[TRAJ_RAMP1] <= 0) pMove->lRampAccMax[TRAJ_RAMP1] = pMove->lMaxAcc;
            wErrorID = MC_TrajJerkCalcAccRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP1], EC_NULL);
            lRamp = Sum3(pMove->lSegDist);
            if(!pMove->bEndless && (lRamp > lDistance))
            {
                if(!bVariableEndVel)
                {
                   /* this never is possible */
                   wErrorID = MC_ERR_CALC_END_VEL_TO_HIGH;
#ifndef MOTIONTEST
                   EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_TrajSetupTrapezeMove(): Velocity not possible because distance is to small. %d < %d", lDistance, lRamp));
#endif
                }
            }
        }
        else if(pMove->lStartVel > pMove->lEndVel)
        {
            /* dec down to EndVel */
            if(pMove->lRampAccMax[TRAJ_RAMP1] >= 0) pMove->lRampAccMax[TRAJ_RAMP1] = -pMove->lMaxDec;
            wErrorID = MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP1], EC_NULL);
            lRamp = Sum3(pMove->lSegDist);

            if(!pMove->bEndless && (lRamp > lDistance))
            {
                /* this never is possible */
                wErrorID = MC_ERR_CALC_END_VEL_TO_LOW;
#ifndef MOTIONTEST
                EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_TrajSetupTrapezeMove(): Velocity not possible because distance is to small. %d < %d", lDistance, lRamp));
#endif
            }
        }
    }

    if((wErrorID == MC_NO_ERROR) && !pMove->bEndless)
    {
        /* what kind of movement ? */
        if((pMove->lStartVel <= pMove->lConstantVel) && (pMove->lConstantVel >= pMove->lEndVel))
        {
            MC_T_REAL       fTopVel;
            EC_T_INT        nCalcLoops = 0;

            /* calculate constant (top) vel */
            if((pMove->lStartVel != 0) || (pMove->lEndVel != 0))
            {
                MC_T_REAL       fVs = (MC_T_REAL)pMove->lStartVel/pAxis->fVelToInc;
                MC_T_REAL       fVe = (MC_T_REAL)pMove->lEndVel/pAxis->fVelToInc;
                fTopVel = (fDeceleration*fVs*fVs + fAcceleration*fVe*fVe + 2*fAcceleration*fDeceleration*(MC_T_REAL)lDistance/pAxis->dwCalcIncPerMM) / (fAcceleration+fDeceleration);
            }
            else
            {
                fTopVel = (2*fAcceleration*fDeceleration*(MC_T_REAL)lDistance/pAxis->dwCalcIncPerMM) / (fAcceleration+fDeceleration);
            }
            fTopVel = sqrt( fTopVel );
            pMove->lConstantVel = (MC_T_VELACC)EC_MIN((fTopVel * pAxis->fVelToInc), pMove->lConstantVel);
            pMove->lConstantVel = EC_MAX(pMove->lConstantVel, 1);
            if(bVariableEndVel)     /* reduce also "end velocity" if enabled */
                pMove->lEndVel = EC_MIN(pMove->lEndVel, pMove->lConstantVel);


            /* check if total distance fits, if not, decrease velocity */
            do
            {
                nCalcLoops++;

                if(pMove->lStartVel < pMove->lConstantVel)
                {
                    /* acc up to ConstantVel */
                    if(pMove->lRampAccMax[TRAJ_RAMP1] <= 0) pMove->lRampAccMax[TRAJ_RAMP1] = pMove->lMaxAcc;
                    wErrorID = MC_TrajJerkCalcAccRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lConstantVel, pMove->lRampAccMax[TRAJ_RAMP1], &lAccRamp1);
                    pMove->lSegDist1to3 = Sum3(pMove->lSegDist);
                }
                else if(pMove->lStartVel > pMove->lConstantVel)
                {
                    /* dec down to ConstantVel */
                    if(pMove->lRampAccMax[TRAJ_RAMP1] >= 0) pMove->lRampAccMax[TRAJ_RAMP1] = -pMove->lMaxDec;
                    wErrorID = MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lConstantVel, pMove->lRampAccMax[TRAJ_RAMP1], &lAccRamp1);
                    pMove->lSegDist1to3 = Sum3(pMove->lSegDist);
                }
                else
                {
                    pMove->lSegDist1to3 = pMove->lSegDist[0] = pMove->lSegDist[1] = pMove->lSegDist[2] = 0;
                    lAccRamp1 = pMove->lStartAcc;
                }

                if(pMove->lConstantVel < pMove->lEndVel)
                {
                    /* acc up to EndVel */
                    if(pMove->lRampAccMax[TRAJ_RAMP2] <= 0) pMove->lRampAccMax[TRAJ_RAMP2] = pMove->lMaxAcc;
                    wErrorID = MC_TrajJerkCalcAccRamp(pMove, TRAJ_RAMP2, lAccRamp1, pMove->lConstantVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP2], EC_NULL);
                    pMove->lSegDist5to7 = Sum3(pMove->lSegDist + 4);
                }
                else if(pMove->lConstantVel > pMove->lEndVel)
                {
                    /* dec down to EndVel */
                    if(pMove->lRampAccMax[TRAJ_RAMP2] >= 0) pMove->lRampAccMax[TRAJ_RAMP2] = -pMove->lMaxDec;
                    wErrorID = MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP2, lAccRamp1, pMove->lConstantVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP2], EC_NULL);
                    pMove->lSegDist5to7 = Sum3(pMove->lSegDist + 4);
                }
                else
                    pMove->lSegDist5to7 = pMove->lSegDist[4] = pMove->lSegDist[5] = pMove->lSegDist[6] = 0;

                /* check if total sum fits */
                lTotalDistSum = pMove->lSegDist1to3 + pMove->lSegDist5to7;
                if(lTotalDistSum <= lDistance)
                {
                    bSuccess = MC_TRUE;
                }
                else
                {

                    /* calculate total move time: Value is not used for trajectory generation! */
                    pMove->dwTotalMoveCycles = pMove->lSegCycles[0] + pMove->lSegCycles[1] + pMove->lSegCycles[2] +
                                               pMove->lSegCycles[4] + pMove->lSegCycles[5] + pMove->lSegCycles[6];
                    fVelAvg = (EC_MAX((MC_T_REAL)lDistance/pMove->dwTotalMoveCycles, 1));

                    /* reduce constant vel */
                    fReduceFactor = ((lTotalDistSum - lDistance)*100.0/lDistance) + 0.5;
                    fReduceFactor *= (MC_T_REAL)pMove->lConstantVel/fVelAvg;
                    fReduceFactor = EC_MAX(fReduceFactor, 1.5);    /* minimum 1.5% */
                    if(pMove->lSegCycles[1] < 2)                 /* if no segment with constant acc we have to reduce lConstantVel more,
                                                                 due to the fact, that acc might be reduced and therefore ramp has same length */
                    {
                        fReduceFactor += 0.5;
                    }
                    if(pMove->lSegCycles[5] < 2)                 /* no segment with constant dec ? */
                    {
                        fReduceFactor += 0.5;
                    }
                    fReduceFactor = EC_MIN(fReduceFactor, 15);   /* maximum 15% */

                    lVelReduce    = (MC_T_VELACC)((fReduceFactor/100.0) * pMove->lConstantVel);
                    lVelReduce++;
                    pMove->lConstantVel  -= lVelReduce;
                    if(bVariableEndVel)     /* reduce also "end velocity" if enabled */
                        pMove->lEndVel = EC_MIN(pMove->lEndVel, pMove->lConstantVel);
                }
            }
            while(!bSuccess && (pMove->lConstantVel > 1) && (pMove->lConstantVel >= pMove->lEndVel) && (pMove->lConstantVel > pMove->lStartVel) &&
                (wErrorID == MC_NO_ERROR) && (nCalcLoops < 20));

            if(!bSuccess &&  (pMove->lConstantVel < pMove->lStartVel))
            {
                pMove->lConstantVel = pMove->lStartVel;
            }
            pMove->lConstantVel  = EC_MAX(pMove->lConstantVel, 1);

#ifdef DEBUG
            if(nCalcLoops > 10)
            {
                EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR,  "ERROR ERROR Jerk: lDist=%d  Vel=%d Acc=%d  Dec=%d  CalcCyc=%d  \n",
                    (EC_T_SDWORD)lDistance, pMove->lConstantVel,  pMove->lMaxAcc, pMove->lMaxDec, nCalcLoops));
            }
#endif
        }
    }

    /* calc ramp 1 */
    if((wErrorID == MC_NO_ERROR) && !bSuccess)
    {
        if(pMove->lStartVel < pMove->lConstantVel)
        {
            /* acc up to ConstantVel */
            if(pMove->lRampAccMax[TRAJ_RAMP1] <= 0) pMove->lRampAccMax[TRAJ_RAMP1] = pMove->lMaxAcc;
            wErrorID = MC_TrajJerkCalcAccRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lConstantVel, pMove->lRampAccMax[TRAJ_RAMP1], &lAccRamp1);
            pMove->lSegDist1to3 = Sum3(pMove->lSegDist);

            if(wErrorID == MC_NO_ERROR)
                wErrorID = MC_TrajCheckRamp1(pMove, lDistance, MC_ERR_CALC_CONSTANT_VEL_TO_HIGH);
        }
        else if(pMove->lStartVel > pMove->lConstantVel)
        {
            /* dec down to ConstantVel */
            if(pMove->lRampAccMax[TRAJ_RAMP1] >= 0) pMove->lRampAccMax[TRAJ_RAMP1] = -pMove->lMaxDec;
            wErrorID = MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lConstantVel, pMove->lRampAccMax[TRAJ_RAMP1], &lAccRamp1);
            pMove->lSegDist1to3 = Sum3(pMove->lSegDist);

            if(wErrorID == MC_NO_ERROR)
                wErrorID = MC_TrajCheckRamp1(pMove, lDistance, MC_ERR_CALC_CONSTANT_VEL_TO_LOW);
        }
        else
        {
            pMove->lSegDist1to3 = pMove->lSegDist[0] = pMove->lSegDist[1] = pMove->lSegDist[2] = pMove->lRampAccMax[TRAJ_RAMP1] = 0;
            lAccRamp1 = pMove->lStartAcc;
        }
    }

    /* calc ramp 2 */
    if((wErrorID == MC_NO_ERROR) && !bSuccess)
    {
RecalcRamp2:
        if(pMove->lConstantVel < pMove->lEndVel)
        {
            /* acc up to EndVel */
            if(wErrorID == MC_NO_ERROR)
            {
                if(pMove->lRampAccMax[TRAJ_RAMP2] <= 0) pMove->lRampAccMax[TRAJ_RAMP2] = pMove->lMaxAcc;
                wErrorID = MC_TrajJerkCalcAccRamp(pMove, TRAJ_RAMP2, lAccRamp1, pMove->lConstantVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP2], EC_NULL);
            }
            pMove->lSegDist5to7 = Sum3(pMove->lSegDist + 4);
            if(wErrorID == MC_NO_ERROR)
                wErrorID = MC_TrajCheckRamp2(pMove, lDistance, MC_ERR_CALC_END_VEL_TO_HIGH);
        }
        else if(pMove->lConstantVel > pMove->lEndVel)
        {
            /* dec down to EndVel */
            if(wErrorID == MC_NO_ERROR)
            {
                if(pMove->lRampAccMax[TRAJ_RAMP2] >= 0) pMove->lRampAccMax[TRAJ_RAMP2] = -pMove->lMaxDec;
                wErrorID = MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP2, lAccRamp1, pMove->lConstantVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP2], EC_NULL);
            }
            pMove->lSegDist5to7 = Sum3(pMove->lSegDist + 4);
            if(wErrorID == MC_NO_ERROR)
                wErrorID = MC_TrajCheckRamp2(pMove, lDistance, MC_ERR_CALC_END_VEL_TO_LOW);
        }
        else
            pMove->lSegDist5to7 = pMove->lSegDist[4] = pMove->lSegDist[5] = pMove->lSegDist[6] = pMove->lRampAccMax[TRAJ_RAMP2] = 0;
    }

    if((wErrorID == MC_NO_ERROR) && !pMove->bEndless)
    {
        lTotalDistSum = pMove->lSegDist1to3 + pMove->lSegDist5to7;
        if(lTotalDistSum > lDistance)
        {
            wErrorID = MC_ERR_CALC_RAMPS_TO_BIG;
#ifndef MOTIONTEST
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_TrajSetupTrapezeMove(): Sum of ramps is bigger than distance. %d < %d\n", lDistance, lTotalDistSum));
#endif
        }
    }

    if((wErrorID == MC_NO_ERROR) && !pMove->bEndless)
    {
        /* calc segment with constant vel */
        pMove->lSegCycles[3] = (EC_T_INT)((lDistance - lTotalDistSum) / pMove->lConstantVel);
        pMove->lSegCycles[3] = EC_MAX(pMove->lSegCycles[3], 0);
        pMove->lSegDist[3]   = pMove->lSegCycles[3] * (MC_T_POS_CALC)pMove->lConstantVel;

        switch(nRecalcRampTwoState)
        {
        case 0:     /* check first time */
            nRecalcRampTwoState = 1;
            if((pMove->lSegCycles[3] != 0) && (lAccRamp1 != 0))      /* have we a segment 4 with constant vel ? */
            {
                lOrgAccRamp1 = lAccRamp1;
                lAccRamp1 = 0;                  /* set start acc for ramp 2 to zero */
                goto RecalcRamp2;               /* calc ramp 2 again */
            }
            break;

        case 1:
            /* it may happen in very rare case, that ramp 2 will be longer with lAccRamp1=0 */
            nRecalcRampTwoState = 2;
            if(pMove->lSegCycles[3] == 0)      /* no segment 4  ? */
            {
                lAccRamp1 = lOrgAccRamp1;
                pMove->lRamp2AccCorrection = lAccRamp1;
                goto RecalcRamp2;               /* calc ramp 2 again */
            }
            break;

        default:
            /* done */
            break;
        }

        /* calculate total move time: Value is not used for trajectory generation! */
        pMove->dwTotalMoveCycles = pMove->lSegCycles[0] + pMove->lSegCycles[1] + pMove->lSegCycles[2] + pMove->lSegCycles[3] +
                                   pMove->lSegCycles[4] + pMove->lSegCycles[5] + pMove->lSegCycles[6];
    }

    /* don't start move in case of an error */
    if(wErrorID != MC_NO_ERROR)
    {
        MC_InitMovement(pMove, bEndless, 0);
    }

    return wErrorID;
}


/********************************************************************************/
/** \brief Preparation for movement in PP mode
*
*
* \return N/A
*/
#ifdef EC_MOTION_SUPPORT_PP_MODE
static EC_T_WORD   MC_SetupTrajInDriveMove
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_MOVEMENT*         pMove
    ,MC_T_BOOL              bNewMove            /* if MC_TRUE, initiate a new movement */
    ,MC_T_BOOL              bEndless
    ,MC_T_POS_CALC          lDistance
    ,MC_T_REAL              fMaxVelocity
    ,MC_T_REAL              fAcceleration
    ,MC_T_REAL              fDeceleration)
{
    EC_T_WORD           wErrorID = MC_NO_ERROR;
    MC_T_POS_CALC       lNewDistance;
    MC_T_BOOL           bMoveParaChanged = MC_FALSE;

    if(bNewMove)
    {
        MC_InitMovement(pMove, bEndless, lDistance);
        lDistance = pMove->lTotalDist;

        /* due to integer arithmetic later we have to round the float values to get better results */
        fMaxVelocity = EC_MAX(fMaxVelocity, 1/pAxis->fVelToInc);
        pMove->lConstantVel = EC_MAX((MC_T_VELACC)(fMaxVelocity * pAxis->fVelToInc), 1);

        pMove->lMaxAcc = (MC_T_VELACC)(fAcceleration * pAxis->fAccToInc);
        pMove->lMaxAcc = EC_MAX(pMove->lMaxAcc, 1);
        fAcceleration  = pMove->lMaxAcc / pAxis->fAccToInc;

        pMove->lMaxDec = (MC_T_VELACC)(fDeceleration * pAxis->fAccToInc);
        pMove->lMaxDec = EC_MAX(pMove->lMaxDec, 1);
        fDeceleration  = pMove->lMaxDec / pAxis->fAccToInc;

        pMove->lJerk        = 0;
        pMove->lEndVel    =  0;

        /* we have to save the initial values */
        pMove->fOrgMaxVelocity   = fMaxVelocity;
        pMove->fOrgAcceleration  = fAcceleration;
        pMove->fOrgDeceleration  = fDeceleration;
        pMove->fOrgJerk          = 0;

        if (bEndless)
        {
            pAxis->wProfileOperationMode = DRV_MODE_OP_PROF_VEL;
        }
        else
        {
            pAxis->wProfileOperationMode = DRV_MODE_OP_PROF_POS;
            if(lDistance == 0)
            {
                pMove->lConstantVel = 0;
                return wErrorID;
            }
        }
    }
    else
    {
        /* detect if input parameters are changed (Distance, Velocity, Acceleration, Deceleration */
        if(pMove->lTotalDist != lDistance)
        {
            /* check if new distance is acceptable */
            lNewDistance = (lDistance - pMove->lTotalDist) + pMove->lRemainDist;

            if(lNewDistance < pMove->lSegDist5to7)    /* if stopping is in progress, new distance is not accepted */
            {
                /* Error */
                wErrorID = MC_ERR_CONTUPDATE_DIST_CHANGED;
                EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_TrajSetupTrapezeMove(): Distance not possible because smaller than breakramp. %d < %d", lNewDistance, pMove->lSegDist5to7));
            }
            else
            {
                pMove->lRemainDist += (lDistance - pMove->lTotalDist);
                pMove->lTotalDist = lDistance;
                bMoveParaChanged = MC_TRUE;
            }
        }

        lDistance = pMove->lRemainDist;

        if(pMove->fOrgMaxVelocity != fMaxVelocity)        /* max velocity changed ? */
        {
            pMove->fOrgMaxVelocity = fMaxVelocity;
            bMoveParaChanged = MC_TRUE;
        }

        if(pMove->fOrgAcceleration != fAcceleration)
        {
            pMove->lMaxAcc      =  (MC_T_VELACC)(fAcceleration * pAxis->fAccToInc);
            pMove->lMaxAcc      =  EC_MAX(pMove->lMaxAcc, 1);
            pMove->fOrgAcceleration  = fAcceleration;
            bMoveParaChanged = MC_TRUE;
        }

        if(pMove->fOrgDeceleration != fDeceleration)
        {
            pMove->lMaxDec      =  (MC_T_VELACC)(fDeceleration * pAxis->fAccToInc);
            pMove->lMaxDec      =  EC_MAX(pMove->lMaxDec, 1);
            pMove->fOrgDeceleration  = fDeceleration;
            bMoveParaChanged = MC_TRUE;
        }

        if(!bMoveParaChanged)
            return wErrorID;

        pMove->lConstantVel      =  (MC_T_VELACC)(fMaxVelocity * pAxis->fVelToInc);
        pMove->lConstantVel      =  EC_MAX(pMove->lConstantVel, 1);
        pMove->bRemainStepCalc = MC_FALSE;
    }

    pMove->lStartVel = pMove->lCommandVel;
    pMove->lStartAcc = pMove->lCommandAcc;

    /* don't start move in case of an error */
    if(wErrorID != MC_NO_ERROR)
    {
        MC_InitMovement(pMove, EC_FALSE, 0);
    }

    return wErrorID;
}
#endif


static EC_T_WORD   MC_MoveCalcDistances
    (MC_T_MOVE_CALC_DATA_T* pData
    ,EC_T_BOOL              bFindSolution)          /* if MC_TRUE try to find a solution by decrementing vel */
{
#if (defined INCLUDE_LOG_MESSAGES)
#undef  pEcLogParmsMotion
#define pEcLogParmsMotion pEcLogParmsMotionMoveCalc
#endif

    EC_T_WORD          wErrorID = MC_NO_ERROR;
    MC_T_REAL          fAccAvg = 0;
    MC_T_REAL          fDecAvg = 0;
    MC_T_BOOL          bSuccess = MC_FALSE;
    MC_T_BOOL          bLastTry = MC_FALSE;
    MC_T_INT           nCalcLoops = 0;
    static MC_T_INT    nMaxCalcLoops = 0;

    /* calculate acc ramp again */
    do
    {
        if(pData->bSeg2)
        {
            pData->fVel_2 = pData->fVelocity - pData->fVs - 2*pData->fVel_1;
            pData->fTime_2 = pData->fVel_2/pData->fAccReal;
            if((pData->fVel_2 < 0) || (pData->bSeg1 && (pData->fTime_2 < pData->fCycleTime)))
            {
                pData->fTime_2 = pData->fVel_2 = pData->fDis_2 = 0;
                pData->bSeg2 = MC_FALSE;
            }
        }

        if(!pData->bSeg2 && pData->bSeg1)
        {
            MC_T_REAL fVel_0 = (pData->fVelocity - pData->fVs) / 2;
            if((MC_T_INT64)(fVel_0*10) < MC_T_INT64(pData->fVel_1*10))
            {
                pData->fVel_1 = fVel_0;
                if(pData->fVel_1 < 0 )
                {
                    wErrorID = MC_ERR_CALC_END_VEL_TO_HIGH;
                    pData->fVel_1 = pData->fTime_1 = pData->fDis_1 = pData->fDis_3 = 0;
                    pData->bSeg1 = MC_FALSE;
                }
                else
                {
                    pData->fTime_1 = sqrt(2*pData->fVel_1/pData->fJerk);
                    if(pData->fTime_1 < pData->fCycleTime)
                    {
                        pData->fTime_1 = pData->fVel_1 = pData->fDis_1 = pData->fDis_3 = 0;
                        pData->bSeg1 = MC_FALSE;

                        pData->bSeg2 = MC_TRUE;
                        pData->fVel_2 = pData->fVelocity - pData->fVs;
                        pData->fTime_2 = pData->fVel_2/pData->fAccReal;
                    }
                    else
                    {
                        pData->fAccReal = pData->fTime_1 * pData->fJerk;
                    }
                }
            }
        }

        if(pData->bSeg1)
        {
            pData->fDis_1 = CAL_WEG_ZU_B( pData->fTime_1, pData->fJerk, pData->fVs, 0);
            pData->fDis_3 = CAL_WEG_AB_B( pData->fTime_1, pData->fJerk, (pData->fVs+pData->fVel_1+pData->fVel_2), pData->fAccReal );
        }

        if(pData->bSeg2) pData->fDis_2  = CAL_WEG_KON_B(pData->fAccReal, pData->fTime_2, pData->fVs+pData->fVel_1);

        /* calculate dec ramp again */
        if(pData->bSeg6)
        {
            pData->fVel_6 = pData->fVelocity - pData->fVe - 2*pData->fVel_7;
            pData->fTime_6 = pData->fVel_6/pData->fDecReal;
            if((pData->fVel_6 < 0) || (pData->bSeg7 && (pData->fTime_6 < pData->fCycleTime)))
            {
                pData->fTime_6 = pData->fVel_6 = pData->fDis_6 = 0;
                pData->bSeg6 = MC_FALSE;
            }
        }

        if(!pData->bSeg6 && pData->bSeg7)
        {
            MC_T_REAL fVel_6 = (pData->fVelocity - pData->fVe) / 2;

            if((MC_T_INT64)(fVel_6*10) < MC_T_INT64(pData->fVel_7*10))
            {
                pData->fVel_7 = fVel_6;
                if(pData->fVel_7 < 0 )
                {
                    wErrorID = MC_ERR_CALC_END_VEL_TO_HIGH;
                    pData->fVel_7 = pData->fTime_7 = pData->fDis_5 = pData->fDis_7 = 0;
                }
                else
                {
                    pData->fTime_7 = sqrt(2*pData->fVel_7/pData->fJerk);
                    if(pData->fTime_7 < pData->fCycleTime)
                    {
                        pData->fTime_7 = pData->fVel_7 = pData->fDis_5 = pData->fDis_7 = 0;
                        pData->bSeg7 = MC_FALSE;

                        pData->bSeg6 = MC_TRUE;
                        pData->fVel_6 = pData->fVelocity - pData->fVe;
                        pData->fTime_6 = pData->fVel_6/pData->fDecReal;
                    }
                    else
                    {
                        pData->fDecReal = pData->fTime_7 * pData->fJerk;
                    }
                }
            }
        }

        if(pData->bSeg7)
        {
            pData->fDis_5 = CAL_WEG_AB_B( pData->fTime_7, pData->fJerk, (pData->fVe+pData->fVel_7+pData->fVel_6), pData->fDecReal );
            pData->fDis_7 = CAL_WEG_ZU_B( pData->fTime_7, pData->fJerk, pData->fVe, 0);
        }
        if(pData->bSeg6) pData->fDis_6 = CAL_WEG_KON_B(pData->fDecReal, pData->fTime_6, pData->fVel_7+pData->fVe);

        pData->fDis_4 = pData->fDistance - pData->fDis_1 - pData->fDis_3 - pData->fDis_5 - pData->fDis_7;
        if(pData->bSeg2) pData->fDis_4-= pData->fDis_2;
        if(pData->bSeg6) pData->fDis_4-= pData->fDis_6;

        if(-pData->fDis_4 < MAX_DISTANCE_SEGMENT4)
        {
            bSuccess = MC_TRUE;
            break;
        }
        else
        {
            if(pData->bSeg2)
            {
                fAccAvg = pData->fAccReal*(pData->fTime_1 + pData->fTime_2)/(2*pData->fTime_1 + pData->fTime_2);
            }
            else
            {
                fAccAvg = pData->fAccReal/2;
            }
            if(pData->bSeg6)
            {
                fDecAvg = pData->fDecReal*(pData->fTime_7 + pData->fTime_6)/(2*pData->fTime_7 + pData->fTime_6);
            }
            else
            {
                fDecAvg = pData->fDecReal/2;
            }
            pData->fPeakVelNoJerk = sqrt((fDecAvg*pData->fVs*pData->fVs + fAccAvg*pData->fVe*pData->fVe + 2*fAccAvg*fDecAvg*pData->fDistance) / (fAccAvg+fDecAvg));

            /* reduce at least 2% */
            pData->fVelocity = EC_MIN(pData->fPeakVelNoJerk, pData->fVelocity * 0.98);

            if(pData->fVelocity <= pData->fMinVelocity)
            {
                pData->fVelocity = pData->fMinVelocity;
                if(bLastTry)
                {
                    wErrorID = MC_ERR_CALC_END_VEL_TO_HIGH;
                }
                else
                {
                    bLastTry = MC_TRUE;
                }
            }
        }

        nCalcLoops++;
        if(nCalcLoops > nMaxCalcLoops)
        {
            nMaxCalcLoops = nCalcLoops;
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR ERROR Maximum calc loops increased to %d\n", nMaxCalcLoops));
        }

        if((wErrorID == MC_NO_ERROR) && (nCalcLoops > 4))
        {
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR,  "ERROR ERROR Calc loops: Dist=%d  MaxVel=%d  lStartVel=%d  Acc=%d  Dec=%d  Jerk=%d  Loops=%d\n",
                (MC_T_DWORD)pData->fDistance, (MC_T_DWORD)pData->fVelocity, pData->fVs,
                (MC_T_DWORD)pData->fAccReal, (MC_T_DWORD)pData->fDecReal, (MC_T_DWORD)pData->fJerk, nCalcLoops));
        }
    } while(!bSuccess && bFindSolution && (wErrorID == MC_NO_ERROR) && (nCalcLoops <= 10));

    return wErrorID;

#if (defined INCLUDE_LOG_MESSAGES)
#undef  pEcLogParmsMotion
#define pEcLogParmsMotion pEcLogParmsMotionDefault
#endif
}


static EC_T_WORD   MC_MoveCalcMaxVelCont
    (MC_T_MOVE_CALC_DATA_T* pData)
{
#if (defined INCLUDE_LOG_MESSAGES)
#undef  pEcLogParmsMotion
#define pEcLogParmsMotion pEcLogParmsMotionMoveCalc
#endif

    EC_T_WORD          wErrorID = MC_NO_ERROR;
    MC_T_REAL          help_a;
    MC_T_REAL          help_b;
    MC_T_REAL          help_c;
    MC_T_REAL          fHelpX;
    MC_T_BOOL          bSuccess = MC_FALSE;

    /* check which segments may not be available */

    /* calculate maximum velocity without considering jerk. This value is always higher than with jerk */
    pData->fPeakVelNoJerk = pData->fVelocity;
    OsDbgAssert(pData->fVelocity > 0);

    if(pData->fVelocity < pData->fMinVelocity)
    {
        wErrorID = MC_ERR_CALC_END_VEL_TO_HIGH;
        goto Exit;
    }

    pData->bSeg2 = MC_TRUE;
    pData->bSeg6 = MC_TRUE;
    if(pData->fJerk > 0)
    {
        pData->fTime_1 = pData->fAccReal / pData->fJerk;
        pData->bSeg1 = MC_TRUE;
        pData->fVel_1 = CAL_VEL_ZU_B( pData->fTime_1, pData->fJerk, 0 , 0);

        pData->fTime_7 = pData->fDecReal / pData->fJerk;
        pData->bSeg7 = MC_TRUE;
        pData->fVel_7 = CAL_VEL_ZU_B( pData->fTime_7, pData->fJerk, 0 , 0);

        /* check against speed limit */
        if(2*pData->fVel_1+pData->fVs > pData->fVelocity)
        {
            pData->fTime_1 = sqrt((pData->fVelocity-pData->fVs)/pData->fJerk);
            {
                pData->fAccReal = pData->fTime_1 * pData->fJerk;
                pData->fVel_1 = CAL_VEL_ZU_B( pData->fTime_1, pData->fJerk, 0, 0);
                pData->bSeg2 = MC_FALSE;
            }
        }

        if(pData->bSeg2)
        {
            pData->fVel_2 = pData->fVelocity - pData->fVs - 2*pData->fVel_1;
            pData->fTime_2 = pData->fVel_2/pData->fAccReal;
        }

        if(2*pData->fVel_7+pData->fVe > pData->fVelocity)
        {
            pData->fTime_7 = sqrt((pData->fVelocity-pData->fVe)/pData->fJerk);
            {
                pData->fDecReal = pData->fTime_7 * pData->fJerk;
                pData->fVel_7 = CAL_VEL_ZU_B( pData->fTime_7, pData->fJerk, 0 , 0);
                pData->bSeg6 = MC_FALSE;
            }
        }

        if(pData->bSeg6)
        {
            pData->fVel_6 = pData->fVelocity - 2*pData->fVel_7 - pData->fVe;
            pData->fTime_6 = pData->fVel_6/pData->fDecReal;
        }
    }

    wErrorID = MC_MoveCalcDistances(pData, EC_FALSE);
    if(-pData->fDis_4 < MAX_DISTANCE_SEGMENT4)
    {
        bSuccess = MC_TRUE;
    }
    else
    {
        OsDbgAssert(pData->fVelocity > 0);
    }

    if((wErrorID == MC_NO_ERROR) && !bSuccess && (pData->bSeg2 || pData->bSeg6))
    {
        MC_T_REAL   fTempVel;
        /* calc times for segments 2 and 6 */
        if(pData->fVel_2 >= pData->fVel_6)
        {
            /* calculate time segment 2 */
            if(pData->bSeg1 && pData->bSeg6)
            {
                fHelpX = pData->fVs - pData->fVe + ((pData->fAccReal*pData->fAccReal - pData->fDecReal*pData->fDecReal)/pData->fJerk);
                fHelpX /= pData->fDecReal;
                help_a = (pData->fAccReal/2.)*(1. +  pData->fAccReal/pData->fDecReal);
                help_b = 1.5*pData->fAccReal/pData->fJerk*(pData->fAccReal + pData->fDecReal);
                help_b += pData->fVs + pData->fAccReal*fHelpX + (pData->fVe*pData->fAccReal/pData->fDecReal);
                help_c =  pData->fAccReal*(pData->fAccReal*pData->fAccReal/pData->fJerk + 2*pData->fVs);
                help_c += pData->fDecReal*(pData->fDecReal*pData->fDecReal/pData->fJerk + 2*pData->fVe);
                help_c /= pData->fJerk;
                help_c += (fHelpX/2.)*(3*pData->fDecReal*pData->fDecReal/pData->fJerk + pData->fDecReal*fHelpX + 2*pData->fVe);
                help_c -=  pData->fDistance;

                pData->fTime_2 = -help_b + sqrt((help_b*help_b) - (4*help_a*help_c));
                pData->fTime_2 = pData->fTime_2 / (2*help_a);
                if((pData->fTime_2 >= pData->fCycleTime) || !pData->bSeg1)      /* if no Seg 1 a segment time below cycle time is o.k. */
                {
                    pData->fVel_2 = pData->fAccReal * pData->fTime_2;

                    fTempVel = pData->fVs + 2*pData->fVel_1 + pData->fVel_2;
                    if(fTempVel > 2*pData->fVel_7 + pData->fVe)
                    {
                        pData->fVelocity = fTempVel;
                        wErrorID = MC_MoveCalcDistances(pData, EC_FALSE);
                        if(-pData->fDis_4 < MAX_DISTANCE_SEGMENT4)
                        {
                            bSuccess = MC_TRUE;
#ifdef DEBUG
                            if(pData->bSeg6) OsDbgAssert(pData->fDis_4 < pData->fDistance/20);
#endif
                        }
                    }
                }
                else
                {
                    pData->fTime_2 = 0;
                }
            }
        }
        else
        {
            /* calculate time segment 6 */
            if(pData->bSeg7 && pData->bSeg2)
            {
                fHelpX = pData->fVe - pData->fVs + ((pData->fDecReal*pData->fDecReal - pData->fAccReal*pData->fAccReal)/pData->fJerk);
                fHelpX /= pData->fAccReal;
                help_a = (pData->fDecReal/2.)*(1. +  pData->fDecReal/pData->fAccReal);

                help_b = 1.5*pData->fDecReal/pData->fJerk*(pData->fDecReal + pData->fAccReal);
                help_b += pData->fVe + pData->fDecReal*fHelpX + (pData->fVs*pData->fDecReal/pData->fAccReal);
                help_c =  pData->fDecReal*(pData->fDecReal*pData->fDecReal/pData->fJerk + 2*pData->fVe);
                help_c += pData->fAccReal*(pData->fAccReal*pData->fAccReal/pData->fJerk + 2*pData->fVs);
                help_c /= pData->fJerk;
                help_c += (fHelpX/2.)*(3*pData->fAccReal*pData->fAccReal/pData->fJerk + pData->fAccReal*fHelpX + 2*pData->fVs);
                help_c -=  pData->fDistance;

                pData->fTime_6 = -help_b + sqrt((help_b*help_b) - (4*help_a*help_c));
                pData->fTime_6 = pData->fTime_6 / (2*help_a);
                if((pData->fTime_6 >= pData->fCycleTime) || !pData->bSeg7)      /* if no Seg 7 a segment time below cycle time is o.k. */
                {
                    pData->fVel_6 = pData->fDecReal * pData->fTime_6;

                    fTempVel = pData->fVe + 2*pData->fVel_7 + pData->fVel_6;
                    if(fTempVel > 2*pData->fVel_1 + pData->fVs)
                    {
                        pData->fVelocity = fTempVel;
                        wErrorID = MC_MoveCalcDistances(pData, EC_FALSE);
                        if(-pData->fDis_4 < MAX_DISTANCE_SEGMENT4)
                        {
                            bSuccess = MC_TRUE;
#ifdef DEBUG
                            if(pData->bSeg2) OsDbgAssert(pData->fDis_4 < pData->fDistance/20);
#endif
                        }
                    }
                }
                else
                {
                    pData->fTime_6 = 0;
                }
            }
        }

        if(!bSuccess && (wErrorID == MC_NO_ERROR))
        {
            wErrorID = MC_MoveCalcDistances(pData, EC_FALSE);
            if(-pData->fDis_4 < MAX_DISTANCE_SEGMENT4)
            {
                bSuccess = MC_TRUE;
            }
        }
    }

    if(!bSuccess && (wErrorID == MC_NO_ERROR))
    {
        wErrorID = MC_MoveCalcDistances(pData, EC_TRUE);
        if(-pData->fDis_4 < MAX_DISTANCE_SEGMENT4)
            bSuccess = MC_TRUE;
    }

    if(wErrorID == MC_NO_ERROR)
    {
        if(pData->fDis_4 > 0)
        {
#ifdef DEBUG
            pData->fTime_4 = pData->fDis_4/pData->fVelocity;
#endif
        }
        else if(fabs(pData->fDis_4) > MAX_DISTANCE_SEGMENT4 )
        {
            wErrorID = MC_ERR_CALC_DIST_NOT_MATCHING;
#ifndef MOTIONTEST
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_MoveCalcMaxVelCont(): Segment distances not equal to total distance\n"));
#endif
        }
    }

Exit:

    return wErrorID;

#if (defined INCLUDE_LOG_MESSAGES)
#undef  pEcLogParmsMotion
#define pEcLogParmsMotion pEcLogParmsMotionDefault
#endif
}

/********************************************************************************/
/** \brief Preparation for new movement including jerk limited
*
* Example: Jerk = 1, MaxAcc = 3, MaxDec = 3; MaxVel = 15
*
* Acc = 0, 1, 2, 3
* Dec = 0, -1, -2, -3
* Vel =  1, 3, 6,  9 ,  12, 14, 15,  15,  15, 14, 12,  9 ,  6, 3, 1
* Seg   [1-----], [2], [3--------], [4], [5--------], [6], [7------]
* Dist = 135

* \return N/A
*/
static EC_T_WORD   MC_TrajSetupJerkMove
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_MOVEMENT*         pMove
    ,MC_T_BOOL              bNewMove            /* if MC_TRUE, initiate a new movement */
    ,MC_T_BOOL              bEndless
    ,MC_T_POS_CALC          lDistance
    ,MC_T_REAL              fMaxVelocity
    ,MC_T_REAL              fEndVelocity
    ,MC_T_REAL              fAcceleration
    ,MC_T_REAL              fDeceleration
    ,MC_T_REAL              fJerk)
{
    EC_T_WORD           wErrorID = MC_NO_ERROR;
    MC_T_POS_CALC       lNewDistance;
    MC_T_BOOL           bMoveParaChanged = MC_FALSE;
    MC_T_POS_CALC       lTotalDistSum = 0;
    MC_T_BOOL           bSuccess = MC_FALSE;
    EC_T_INT            nCalcLoops = 0;
    MC_T_VELACC         lAccRamp1 = 0, lOrgAccRamp1 = 0;
    MC_T_INT            nRecalcRampTwoState = 0;
    MC_T_BOOL           bVariableEndVel = MC_FALSE;
    MC_T_MOVE_CALC_DATA_T  oData;
    OsMemset(&oData, 0, sizeof(MC_T_MOVE_CALC_DATA_T));
    oData.pLogParms = &pAxis->LogParms;

    if(bNewMove)
    {
        MC_InitMovement(pMove, bEndless, lDistance);
        lDistance = pMove->lTotalDist;

        /* due to integer arithmetic later we have to round the float values to get better results */
        fMaxVelocity = EC_MAX(fMaxVelocity, 1/pAxis->fVelToInc);
        pMove->lConstantVel = EC_MAX((MC_T_VELACC)(fMaxVelocity * pAxis->fVelToInc), 1);
        if(lDistance != 0) pMove->lConstantVel = (MC_T_VELACC)EC_MIN(pMove->lConstantVel, lDistance);

        pMove->lMaxAcc = (MC_T_VELACC)(fAcceleration * pAxis->fAccToInc);
        if(lDistance != 0) pMove->lMaxAcc = (MC_T_VELACC)EC_MIN(pMove->lMaxAcc, lDistance/2);
        pMove->lMaxAcc = EC_MIN(pMove->lMaxAcc, pMove->lConstantVel);
        pMove->lMaxAcc = EC_MAX(pMove->lMaxAcc, 1);
        fAcceleration  = pMove->lMaxAcc / pAxis->fAccToInc;

        pMove->lMaxDec = (MC_T_VELACC)(fDeceleration * pAxis->fAccToInc);
        if(lDistance != 0) pMove->lMaxDec = (MC_T_VELACC)EC_MIN(pMove->lMaxDec, lDistance/2);
        pMove->lMaxDec = EC_MIN(pMove->lMaxDec, pMove->lConstantVel);
        pMove->lMaxDec = EC_MAX(pMove->lMaxDec, 1);
        fDeceleration  = pMove->lMaxDec / pAxis->fAccToInc;

        if(fJerk != 0)
        {
            pMove->lJerk = (MC_T_VELACC)(fJerk * pAxis->fJerkToInc);
            pMove->lJerk = EC_MAX(pMove->lJerk, 1);
            fJerk = (MC_T_REAL)pMove->lJerk / pAxis->fJerkToInc;
        }
        else
            pMove->lJerk = 0;

        pMove->lEndVel    =  EC_MAX((MC_T_VELACC)(fEndVelocity * pAxis->fVelToInc), 0);

        /* we have to save the initial values */
        pMove->fOrgMaxVelocity   = fMaxVelocity;
        pMove->fOrgEndVelocity   = fEndVelocity;
        pMove->fOrgAcceleration  = fAcceleration;
        pMove->fOrgDeceleration  = fDeceleration;
        pMove->fOrgJerk          = fJerk;

        if(!bEndless && (lDistance == 0))
        {
            pMove->lConstantVel = 0;
            return wErrorID;
        }
    }
    else
    {
        /* detect if input parameters are changed (Distance, Velocity, Acceleration, Deceleration */
        if(pMove->lTotalDist != lDistance)
        {
            /* check if new distance is acceptable */
            lNewDistance = (lDistance - pMove->lTotalDist) + pMove->lRemainDist;

            if(lNewDistance < pMove->lSegDist5to7)    /* if stopping is in progress, new distance is not accepted */
            {
                /* Error */
                wErrorID = MC_ERR_CONTUPDATE_DIST_CHANGED;
                EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_TrajSetupJerkMove(): Distance not possible because smaller than breakramp. %d < %d", lNewDistance, pMove->lSegDist5to7));
            }
            else
            {
                pMove->lRemainDist += (lDistance - pMove->lTotalDist);
                pMove->lTotalDist = lDistance;
                bMoveParaChanged = MC_TRUE;
            }
        }

        lDistance = pMove->lRemainDist;

        if(pMove->fOrgMaxVelocity != fMaxVelocity)        /* max velocity changed ? */
        {
            pMove->fOrgMaxVelocity = fMaxVelocity;
            bMoveParaChanged = MC_TRUE;
        }

        if(pMove->fOrgEndVelocity != fEndVelocity)        /* end velocity changed ? */
        {
            pMove->lEndVel      =  (MC_T_VELACC)(fEndVelocity * pAxis->fVelToInc);
            pMove->lEndVel      =  EC_MAX(pMove->lEndVel, 0);
            pMove->fOrgEndVelocity = fEndVelocity;
            bMoveParaChanged = MC_TRUE;
            bVariableEndVel = MC_TRUE;          /* we can change the "end velocity" already calculated in the previous calc. */
        }

        if(pMove->fOrgAcceleration != fAcceleration)
        {
            pMove->lMaxAcc      =  (MC_T_VELACC)(fAcceleration * pAxis->fAccToInc);
            pMove->lMaxAcc      =  EC_MAX(pMove->lMaxAcc, 1);
            pMove->fOrgAcceleration  = fAcceleration;
            bMoveParaChanged = MC_TRUE;
        }

        if(pMove->fOrgDeceleration != fDeceleration)
        {
            pMove->lMaxDec      =  (MC_T_VELACC)(fDeceleration * pAxis->fAccToInc);
            pMove->lMaxDec      =  EC_MAX(pMove->lMaxDec, 1);
            pMove->fOrgDeceleration  = fDeceleration;
            bMoveParaChanged = MC_TRUE;
        }

        if(!bMoveParaChanged)
            return wErrorID;

        pMove->lConstantVel =  (MC_T_VELACC)(fMaxVelocity * pAxis->fVelToInc);
        pMove->lConstantVel =  EC_MAX(pMove->lConstantVel, 1);
        pMove->bRemainStepCalc = MC_FALSE;
    }

    pMove->lStartVel = pMove->lCommandVel;
    pMove->lStartAcc = pMove->lCommandAcc;
    lAccRamp1 = pMove->lStartAcc;

    /* check if move is in general possible */
    if(!pMove->bEndless)
    {
        MC_T_POS_CALC   lRamp;

        if(pMove->lStartVel < pMove->lEndVel)
        {
            /* acc up to ConstantVel */
            if(pMove->lRampAccMax[TRAJ_RAMP1] <= 0) pMove->lRampAccMax[TRAJ_RAMP1] = pMove->lMaxAcc;
            wErrorID = MC_TrajJerkCalcAccRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP1], EC_NULL);
            lRamp = Sum3(pMove->lSegDist);
            if(lRamp > lDistance)
            {
                if(!bVariableEndVel)
                {
                   /* this never is possible */
                   wErrorID = MC_ERR_CALC_END_VEL_TO_HIGH;
#ifndef MOTIONTEST
                   EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_TrajSetupJerkMove(): Velocity not possible because distance is to small. %d < %d", lDistance, lRamp));
#endif
                }
            }
        }
        else if(pMove->lStartVel > pMove->lEndVel)
        {
            /* dec down to EndVel */
            if(pMove->lRampAccMax[TRAJ_RAMP1] >= 0) pMove->lRampAccMax[TRAJ_RAMP1] = -pMove->lMaxDec;
            wErrorID = MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP1], EC_NULL);
            lRamp = Sum3(pMove->lSegDist);
            if(lRamp > lDistance)
            {
                /* this never is possible */
                wErrorID = MC_ERR_CALC_END_VEL_TO_LOW;
#ifndef MOTIONTEST
                EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_TrajSetupJerkMove(): Velocity not possible because distance is to small. %d < %d", lDistance, lRamp));
#endif
            }
        }
    }
    if(wErrorID == MC_NO_ERROR)
    {
        /* what kind of movement ? */
        if(!pMove->bEndless && (pMove->lStartVel <= pMove->lConstantVel) && (pMove->lConstantVel > pMove->lEndVel))
        {
            /* calculate constant (top) vel */

            oData.fDistance = (MC_T_REAL)lDistance / pAxis->dwCalcIncPerMM;
            oData.fVelocity = fMaxVelocity;
            oData.fJerk = fJerk;
            oData.fAccReal = fAcceleration;
            oData.fDecReal = fDeceleration;
            oData.fCycleTime = pAxis->dwCycleTime/1E6;

            {
                MC_T_REAL       fVs = (MC_T_REAL)pMove->lStartVel/pAxis->fVelToInc;
                MC_T_REAL       fVe = (MC_T_REAL)pMove->lEndVel/pAxis->fVelToInc;

                oData.fVs = fVs;
                oData.fVe = fVe;
                oData.fMinVelocity = EC_MAX(fVs, fVe);

                wErrorID = MC_MoveCalcMaxVelCont(&oData);
            }
            pMove->lConstantVel = (MC_T_VELACC)EC_MIN((oData.fVelocity * pAxis->fVelToInc), pMove->lConstantVel);
            pMove->lConstantVel = EC_MAX(pMove->lConstantVel, 1);
            pMove->lMaxAcc = EC_MAX((MC_T_POSITION)(oData.fAccReal * pAxis->fAccToInc), 1);
            pMove->lMaxDec = EC_MAX((MC_T_POSITION)(oData.fDecReal * pAxis->fAccToInc), 1);
        }
    }


    /* validate constant vel with increment based calc */
    if((wErrorID == MC_NO_ERROR))
    {
        /* check if total distance fits, if not, decrease velocity */
        do
        {
            nCalcLoops++;

            if(pMove->lStartVel < pMove->lConstantVel)
            {
                /* acc up to ConstantVel */
                if(pMove->lRampAccMax[TRAJ_RAMP1] <= 0) pMove->lRampAccMax[TRAJ_RAMP1] = pMove->lMaxAcc;
                wErrorID = MC_TrajJerkCalcAccRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lConstantVel, pMove->lRampAccMax[TRAJ_RAMP1], &lAccRamp1);
                pMove->lSegDist1to3 = Sum3(pMove->lSegDist);
            }
            else if(pMove->lStartVel > pMove->lConstantVel)
            {
                /* dec down to ConstantVel */
                if(pMove->lRampAccMax[TRAJ_RAMP1] >= 0) pMove->lRampAccMax[TRAJ_RAMP1] = -pMove->lMaxDec;
                wErrorID = MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lConstantVel, pMove->lRampAccMax[TRAJ_RAMP1], &lAccRamp1);
                pMove->lSegDist1to3 = Sum3(pMove->lSegDist);
            }
            else
            {
                pMove->lSegDist1to3 = pMove->lSegDist[0] = pMove->lSegDist[1] = pMove->lSegDist[2] = 0;
                lAccRamp1 = pMove->lStartAcc;
            }

            if(pMove->lConstantVel < pMove->lEndVel)
            {
                /* acc up to EndVel */
                if(pMove->lRampAccMax[TRAJ_RAMP2] <= 0) pMove->lRampAccMax[TRAJ_RAMP2] = pMove->lMaxAcc;
                wErrorID = MC_TrajJerkCalcAccRamp(pMove, TRAJ_RAMP2, lAccRamp1, pMove->lConstantVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP2], EC_NULL);
                pMove->lSegDist5to7 = Sum3(pMove->lSegDist + 4);
            }
            else if(pMove->lConstantVel > pMove->lEndVel)
            {
                /* dec down to EndVel */
                if(pMove->lRampAccMax[TRAJ_RAMP2] >= 0) pMove->lRampAccMax[TRAJ_RAMP2] = -pMove->lMaxDec;
                wErrorID = MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP2, lAccRamp1, pMove->lConstantVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP2], EC_NULL);
                pMove->lSegDist5to7 = Sum3(pMove->lSegDist + 4);
            }
            else
                pMove->lSegDist5to7 = pMove->lSegDist[4] = pMove->lSegDist[5] = pMove->lSegDist[6] = 0;

            /* check if total sum fits */
            lTotalDistSum = pMove->lSegDist1to3 + pMove->lSegDist5to7;
            if(pMove->bEndless || (lTotalDistSum <= lDistance))
            {
                bSuccess = MC_TRUE;
            }
            else
            {
                MC_T_VELACC     lVelReduce = 0;
                MC_T_REAL       fReduceFactor;
                MC_T_REAL       fVelAvg;        /* average velocity */

                /* calculate total move time: Value is not used for trajectory generation! */
                pMove->dwTotalMoveCycles = pMove->lSegCycles[0] + pMove->lSegCycles[1] + pMove->lSegCycles[2] +
                                           pMove->lSegCycles[4] + pMove->lSegCycles[5] + pMove->lSegCycles[6];

                fVelAvg = (EC_MAX((MC_T_REAL)lDistance/pMove->dwTotalMoveCycles, 1));

                /* reduce constant vel */
                fReduceFactor = ((lTotalDistSum - lDistance)*100.0/lDistance) + 0.5;
                fReduceFactor *= (MC_T_REAL)pMove->lConstantVel/fVelAvg;
                fReduceFactor = EC_MAX(fReduceFactor, 1.5);    /* minimum 1.5% */
                if(pMove->lSegCycles[1] < 2)                 /* if no segment with constant acc we have to reduce lConstantVel more,
                                                             due to the fact, that acc might be reduced and therefore ramp has same length */
                {
                    fReduceFactor += 0.5;
                }
                if(pMove->lSegCycles[5] < 2)                 /* no segment with constant dec ? */
                {
                    fReduceFactor += 0.5;
                }
                fReduceFactor = EC_MIN(fReduceFactor, 15);   /* maximum 15% */

                lVelReduce    = (MC_T_VELACC)((fReduceFactor/100.0) * pMove->lConstantVel);
                lVelReduce++;
                pMove->lConstantVel -= lVelReduce;
                if(bVariableEndVel)     /* reduce also "end velocity" if enabled */
                    pMove->lEndVel = EC_MIN(pMove->lEndVel, pMove->lConstantVel);
            }
        }
        while(!bSuccess && (pMove->lConstantVel > 1) && (pMove->lConstantVel >= pMove->lEndVel) && (pMove->lConstantVel > pMove->lStartVel) &&
            (wErrorID == MC_NO_ERROR) && (nCalcLoops < 20));

        if(!bSuccess)
        {
            pMove->lMaxAcc = EC_MAX((MC_T_POSITION)(oData.fAccReal * pAxis->fAccToInc), 1);
            pMove->lMaxDec = EC_MAX((MC_T_POSITION)(oData.fDecReal * pAxis->fAccToInc), 1);
            if(pMove->lConstantVel < pMove->lStartVel) pMove->lConstantVel = pMove->lStartVel;
            if(pMove->lConstantVel < pMove->lEndVel)   pMove->lConstantVel = pMove->lEndVel;
        }
        pMove->lConstantVel  = EC_MAX(pMove->lConstantVel, 1);

#ifdef DEBUG
        if((wErrorID == MC_NO_ERROR) && (nCalcLoops > 10))
        {
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR ERROR Calc loops: Dist=%d  MaxVel=%d  lStartVel=%d  Acc=%d  Dec=%d  Jerk=%d  Loops=%d\n",
                (MC_T_DWORD)lDistance, (MC_T_DWORD)fMaxVelocity, pMove->lStartVel,
                (MC_T_DWORD)fAcceleration, (MC_T_DWORD)fDeceleration, (MC_T_DWORD)fJerk, nCalcLoops));
        }
#endif
    }

    /* calc ramp 1 */
    if((wErrorID == MC_NO_ERROR) && !bSuccess)
    {
        if(pMove->lStartVel < pMove->lConstantVel)
        {
            /* acc up to ConstantVel */
            if(pMove->lRampAccMax[TRAJ_RAMP1] <= 0) pMove->lRampAccMax[TRAJ_RAMP1] = pMove->lMaxAcc;
            wErrorID = MC_TrajJerkCalcAccRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lConstantVel, pMove->lRampAccMax[TRAJ_RAMP1], &lAccRamp1);
            pMove->lSegDist1to3 = Sum3(pMove->lSegDist);

            if(wErrorID == MC_NO_ERROR)
                wErrorID = MC_TrajCheckRamp1(pMove, lDistance, MC_ERR_CALC_CONSTANT_VEL_TO_HIGH);
        }
        else if(pMove->lStartVel > pMove->lConstantVel)
        {
            /* dec down to ConstantVel */
            if(pMove->lRampAccMax[TRAJ_RAMP1] >= 0) pMove->lRampAccMax[TRAJ_RAMP1] = -pMove->lMaxDec;
            wErrorID = MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP1, pMove->lStartAcc, pMove->lStartVel, pMove->lConstantVel, pMove->lRampAccMax[TRAJ_RAMP1], &lAccRamp1);
            pMove->lSegDist1to3 = Sum3(pMove->lSegDist);

            if(wErrorID == MC_NO_ERROR)
                wErrorID = MC_TrajCheckRamp1(pMove, lDistance, MC_ERR_CALC_CONSTANT_VEL_TO_LOW);
        }
        else
        {
            pMove->lSegDist1to3 = pMove->lSegDist[0] = pMove->lSegDist[1] = pMove->lSegDist[2] = 0;
            lAccRamp1 = pMove->lStartAcc;
        }
    }

    /* calc ramp 2 */
    if((wErrorID == MC_NO_ERROR) && !bSuccess)
    {
RecalcRamp2:
        if(pMove->lConstantVel < pMove->lEndVel)
        {
            /* acc up to EndVel */
            if(wErrorID == MC_NO_ERROR)
            {
                if(pMove->lRampAccMax[TRAJ_RAMP2] <= 0) pMove->lRampAccMax[TRAJ_RAMP2] = pMove->lMaxAcc;
                wErrorID = MC_TrajJerkCalcAccRamp(pMove, TRAJ_RAMP2, lAccRamp1, pMove->lConstantVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP2], EC_NULL);
                pMove->lSegDist5to7 = Sum3(pMove->lSegDist + 4);
                if(wErrorID == MC_NO_ERROR)
                    wErrorID = MC_TrajCheckRamp2(pMove, lDistance, MC_ERR_CALC_END_VEL_TO_HIGH);
            }
        }
        else if(pMove->lConstantVel > pMove->lEndVel)
        {
            /* dec down to EndVel */
            if(wErrorID == MC_NO_ERROR)
            {
                if(pMove->lRampAccMax[TRAJ_RAMP2] >= 0) pMove->lRampAccMax[TRAJ_RAMP2] = -pMove->lMaxDec;
                wErrorID = MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP2, lAccRamp1, pMove->lConstantVel, pMove->lEndVel, pMove->lRampAccMax[TRAJ_RAMP2], EC_NULL);
                pMove->lSegDist5to7 =Sum3(pMove->lSegDist + 4);
                if(wErrorID == MC_NO_ERROR)
                    wErrorID = MC_TrajCheckRamp2(pMove, lDistance, MC_ERR_CALC_END_VEL_TO_LOW);
            }
        }
        else
            pMove->lSegDist5to7 = pMove->lSegDist[4] = pMove->lSegDist[5] = pMove->lSegDist[6] = 0;
    }

    if((wErrorID == MC_NO_ERROR) && !pMove->bEndless)
    {
        lTotalDistSum = pMove->lSegDist1to3 + pMove->lSegDist5to7;
        if((lTotalDistSum > lDistance) && (lDistance > 1))
        {
            wErrorID = MC_ERR_CALC_RAMPS_TO_BIG;
#ifndef MOTIONTEST
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_TrajSetupJerkMove(): Sum of ramps is bigger than distance. %d < %d\n", lDistance, lTotalDistSum));
#endif
        }
    }

    if((wErrorID == MC_NO_ERROR) && !pMove->bEndless)
    {
        /* calc segment with constant vel */
        pMove->lSegCycles[3] = (EC_T_INT)((lDistance - lTotalDistSum) / pMove->lConstantVel);
        pMove->lSegCycles[3] = EC_MAX(pMove->lSegCycles[3], 0);
        pMove->lSegDist[3]   = pMove->lSegCycles[3] * (MC_T_POS_CALC)pMove->lConstantVel;

        switch(nRecalcRampTwoState)
        {
        case 0:     /* check first time */
            nRecalcRampTwoState = 1;
            if((pMove->lSegCycles[3] != 0) && (lAccRamp1 != 0))      /* have we a segment 4 with constant vel ? */
            {
                lOrgAccRamp1 = lAccRamp1;
                lAccRamp1 = 0;                  /* set start acc for ramp 2 to zero */
                goto RecalcRamp2;               /* calc ramp 2 again */
            }
            break;

        case 1:
            /* it may happen in very rare case, that ramp 2 will be longer with lAccRamp1=0 */
            nRecalcRampTwoState = 2;
            if(pMove->lSegCycles[3] == 0)      /* no segment 4  ? */
            {
                lAccRamp1 = lOrgAccRamp1;
                pMove->lRamp2AccCorrection = lAccRamp1;
                goto RecalcRamp2;               /* calc ramp 2 again */
            }
            break;

        default:
            /* done */
            break;
        }

        /* calculate total move time: Value is not used for trajectory generation! */
        pMove->dwTotalMoveCycles = pMove->lSegCycles[0] + pMove->lSegCycles[1] + pMove->lSegCycles[2] + pMove->lSegCycles[3] +
                                   pMove->lSegCycles[4] + pMove->lSegCycles[5] + pMove->lSegCycles[6];
    }

    /* don't start move in case of an error */
    if(wErrorID != MC_NO_ERROR)
    {
        MC_InitMovement(pMove, bEndless, 0);
    }

    return wErrorID;
}



/********************************************************************************/
/** \brief Calculate break distance
*
* \return: error code
*/
static EC_T_WORD   MC_TrajBreakDist
    (MC_T_MOVEMENT*         pMove)
{
    EC_T_WORD       wErrorID = MC_NO_ERROR;

    pMove->lRemainStep     = 0;
    pMove->bRemainStepCalc = MC_FALSE;
    pMove->lStartAcc = pMove->lCommandAcc;
    pMove->lStartVel = pMove->lCommandVel;
    pMove->lEndVel = 0;
    pMove->bEndless = MC_FALSE;

    MC_TrajJerkCalcDecRamp(pMove, TRAJ_RAMP2, pMove->lStartAcc, pMove->lCommandVel, pMove->lEndVel, -pMove->lMaxDec, EC_NULL);

    /* calculate break distance */
    pMove->lSegDist5to7 = Sum3(pMove->lSegDist + 4);

    return wErrorID;
}


/********************************************************************************/
/** \brief Check if target position is reached
*
*
* \return: error code
*/
static EC_T_WORD   MC_CheckTargetPosReached
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_BOOL*             pbDone)
{
   EC_T_WORD            wErrorID = MC_NO_ERROR;
   MC_T_VELACC          lNextVel = 0;

   if(pAxis->oMove.lEndVel != 0)
   {
      lNextVel = MC_TrajFollowingSetPoint(&pAxis->oMove);
   }
   if ((pAxis->oMove.lRemainDist <= 0) || (pAxis->oMove.lRemainDist < lNextVel))
   {
      *pbDone = MC_TRUE;
   }

   if (pAxis->eAxisType == MC_AXIS_TYPE_REAL_ALL && pAxis->eProfile == MC_T_AXIS_PROFILE_DS402)
   {
      if(pAxis->wStatusWord & DRV_STAT_FOLLOW_ERR)
          wErrorID = MC_ERR_FOLLOWING;

      /* not supported
      if(pAxis->bDriveFollowBitSupp)
      {
          if((pAxis->wStatusWord & DRV_STAT_FOLLOW_CMD_VALUE) == 0)
            wErrorID = MC_ERR_NOT_FOLLOW_CMD_VALUE;
      } */
   }

   return wErrorID;
}

// Homing supported only in PP mode
#ifdef EC_MOTION_SUPPORT_PP_MODE
/********************************************************************************/
/** \brief Check if homing done
*
* \return: error code
*/
static EC_T_WORD   MC_CheckHomingDone
    (MC_T_AXIS_REF*         pAxis
    , MC_T_BOOL*            pbDone)
{
    EC_T_WORD wErrorID = MC_NO_ERROR;

    if (pAxis->eAxisType == MC_AXIS_TYPE_REAL_ALL
        && pAxis->eProfile == MC_T_AXIS_PROFILE_DS402
        && (pAxis->wStatusWord & DRV_STAT_FOLLOW_ERR))
    {
        wErrorID = MC_ERR_FOLLOWING;
    }

    if (pAxis->oMove.eProfileState == eProfileStatus_Idle)
    {
        *pbDone = MC_TRUE;
    }
    else if (pAxis->oMove.eProfileState == eProfileStatus_Error)
    {
        wErrorID = MC_ERR_HOMING;
    }

    return wErrorID;
}
#endif

#ifdef EC_MOTION_SUPPORT_PP_MODE
/********************************************************************************/
/** \brief Check if target position is reached
*
*
   In PP Mode warten bis Mode acknowelded kommt
* \return: error code
*/
static EC_T_WORD   MC_CheckTargetPosReachedTrajInDrive
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_BOOL*             pbDone)
{
   EC_T_WORD wErrorID = MC_NO_ERROR;

   if (pAxis->eAxisType == MC_AXIS_TYPE_REAL_ALL
     && pAxis->eProfile == MC_T_AXIS_PROFILE_DS402
     && (pAxis->wStatusWord & DRV_STAT_FOLLOW_ERR))
   {
      wErrorID = MC_ERR_FOLLOWING;
   }

   if (pAxis->oMove.eProfileState == eProfileStatus_Idle)
   {
      *pbDone = MC_TRUE;
   }

   return wErrorID;
}
#endif

/********************************************************************************/
/** \brief Check if target velocity is reached
*
* \return: error code
*/
static EC_T_VOID   MC_CheckTargetVelReached
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_BOOL*             pbInVelocity)
{
    MC_T_VELACC             lActVel;

    lActVel = (pAxis->dwDirection == MC_DIR_POSITIVE) ? pAxis->lActualVelocity : -pAxis->lActualVelocity;

    if(*pbInVelocity == MC_FALSE)
    {
        /* at least 95 % of commanded vel reached ? */
        if(lActVel >= (MC_T_REAL)pAxis->oMove.lConstantVel*0.95)
        {
            *pbInVelocity = MC_TRUE;
        }
    }
    else
    {
        if(lActVel <= (MC_T_REAL)pAxis->oMove.lConstantVel*0.85)
        {
            *pbInVelocity = MC_FALSE;
        }
    }
}


/********************************************************************************/
/** \brief Get the drive input data (actual position, velocity, torque)
*
* \return: error code
*/
static EC_T_WORD   MC_DriveGetInputs
    (MC_T_AXIS_REF*         pAxis)
{
    EC_T_SDWORD     lSignedPos;

    if (pAxis->plPdActualPosition)
    {
        lSignedPos = EC_GET_FRM_DWORD(pAxis->plPdActualPosition);

        /* detect overrun */
        if(pAxis->lLastActualPosDrive != 0)
        {
            /* now negative and previous value very positive ?*/
            if((lSignedPos < 0) && (pAxis->lLastActualPosDrive > 0x70000000))
            {
                pAxis->nActualPosOverrunCounter++;
                /* OsDbgMsg("nActualPosOverrunCounter inc: %d\n", pAxis->nActualPosOverrunCounter); */
            }

            /* now positive and previous value very negative ?*/
            else if((lSignedPos > 0) && (pAxis->lLastActualPosDrive < -0x70000000))
            {
                pAxis->nActualPosOverrunCounter--;
                /* OsDbgMsg("nActualPosOverrunCounter dec: %d\n", pAxis->nActualPosOverrunCounter); */
            }
        }
        pAxis->lLastActualPosDrive = lSignedPos;    /* store for next cycle */

        pAxis->lActualPosRaw = (pAxis->nActualPosOverrunCounter * EC_MAKEQWORD(0x00000001, 0x00000000)) + lSignedPos;
        pAxis->lActualPosRaw = pAxis->lActualPosRaw*pAxis->lIncFactor;
    }
    else
    {
        pAxis->lActualPosRaw = pAxis->lCommandedPosition - pAxis->lPosOffset;
    }

    pAxis->lActualPosition = pAxis->lActualPosRaw + pAxis->lPosOffset;

    /* calculate actual velocity */
    pAxis->lActualVelocity = (MC_T_VELACC)(pAxis->lActualPosition - pAxis->lOldActualPosition);
    pAxis->lOldActualPosition = pAxis->lActualPosition;

    /* Torque */
    if (pAxis->psPdActualTorque)
    {
        pAxis->sActualTorque = EC_GET_FRM_WORD(pAxis->psPdActualTorque);
    }

    return MC_NO_ERROR;
}


/********************************************************************************/
/** \brief Set the target step to the drive
*
* \return: error code
*/
EC_T_WORD   MC_DriveSetTargetStep
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_POSITION          lTargetPosStep
    ,MC_T_VELACC            lTargetVel
    ,MC_T_VELACC            lTargetAcc)
{
   EC_T_WORD               wErrorID = MC_NO_ERROR;
   MC_T_POS_CALC           lTargetPosition;
   EC_T_SDWORD             lTargetVelocity = 0;
   MC_T_VELACC             lTargetTorque = 0;

   lTargetPosition = pAxis->lCommandedPosition;
   if(lTargetPosStep != 0)
   {
      if(pAxis->dwDirection == MC_DIR_POSITIVE)
      {
         lTargetPosition += lTargetPosStep;
      }
      else
      {
         lTargetPosition -= lTargetPosStep;
      }
   }

   switch(pAxis->eOperationMode)
   {
   case OPMODE_CSP:
      if (pAxis->plPdTargetPosition)
      {
         EC_SET_FRM_DWORD(pAxis->plPdTargetPosition,
            (EC_T_DWORD)((lTargetPosition - pAxis->lPosOffset)/pAxis->lIncFactor));
      }

      if (pAxis->plPdVelocityOffset)               /* velocity feed forward */
      {
          lTargetVel *= pAxis->dwVelocityGain;     /* multiply with gain */
          lTargetVel /= pAxis->lIncFactor;
          EC_SET_FRM_DWORD(pAxis->plPdVelocityOffset, (EC_T_DWORD)lTargetVel);
      }
      if (pAxis->psPdTorqueOffset)                 /* torque feed forward */
      {
          lTargetAcc *= pAxis->dwTorqueGain;       /* multiply with gain */
          lTargetAcc /= pAxis->lIncFactor;
          EC_SET_FRM_WORD(pAxis->psPdTorqueOffset, (EC_T_WORD)lTargetAcc);
      }
      break;

   case OPMODE_CSV:
      {

         if (pAxis->plPdTargetVelocity)
         {
            MC_T_REAL               fVelFactor;
#if 0
            /* calculate velocity */
            lTargetVelocity = MC_T_VELACC((lTargetPosition - pAxis->lActualPosition)/pAxis->lIncFactor);
            lTargetVelocity *= pAxis->dwVelocityGain;     /* multiply with gain */
            EC_SET_FRM_DWORD(pAxis->plPdTargetVelocity, lTargetVelocity);
#else
            fVelFactor = pAxis->lVeltoAcc/(MC_T_REAL)pAxis->lIncFactor;
            if(pAxis->dwDirection == MC_DIR_POSITIVE)
            {
                EC_SET_FRM_DWORD(pAxis->plPdTargetVelocity, (MC_T_SDWORD)(lTargetPosStep*fVelFactor));
            }
            else
            {
               EC_SET_FRM_DWORD(pAxis->plPdTargetVelocity, -(MC_T_SDWORD)(lTargetPosStep*fVelFactor));
            }
#endif
         }
      }
      if (pAxis->psPdTorqueOffset)                 /* torque feed forward */
      {
          lTargetAcc *= pAxis->dwTorqueGain;       /* multiply with gain */
          lTargetAcc /= pAxis->lIncFactor;
          EC_SET_FRM_WORD(pAxis->psPdTorqueOffset, (EC_T_WORD)lTargetAcc);
      }
      break;

   case OPMODE_CST:
      {
         lTargetVelocity = MC_T_VELACC((lTargetPosition - pAxis->lActualPosition) / pAxis->lIncFactor);

         lTargetTorque = (lTargetVelocity - pAxis->lActualVelocity) / pAxis->lIncFactor;
         lTargetTorque = lTargetTorque / pAxis->lVeltoAcc;
      }
      break;

#ifdef EC_MOTION_SUPPORT_PP_MODE
   case OPMODE_PP:
   case OPMODE_HOMING:
      wErrorID = MC_ERR_INVALID_OPERATION_MODE;
      OsDbgAssert(MC_FALSE);
      break;
#endif

   default:
      wErrorID = MC_ERR_INVALID_OPERATION_MODE;
      OsDbgAssert(MC_FALSE);
      break;
   }

   pAxis->lCommandedPosition = lTargetPosition;

   return wErrorID;
}


/********************************************************************************/
/** \brief Set the target position to the drive
*
* \return: error code
*/
static EC_T_WORD   MC_DriveSetTargetPosition
    (MC_T_AXIS_REF*         pAxis
    ,MC_T_POS_CALC          lTargetPosition)    /* absolute target position */
{
    EC_T_WORD               wErrorID = MC_NO_ERROR;

    pAxis->lCommandedPosition = lTargetPosition;

    switch(pAxis->eOperationMode)
    {
#ifdef EC_MOTION_SUPPORT_PP_MODE
    case OPMODE_PP:
    case OPMODE_HOMING:
#endif
    case OPMODE_CSP:
        if (pAxis->plPdTargetPosition)
            EC_SET_FRM_DWORD(pAxis->plPdTargetPosition, (EC_T_DWORD)(lTargetPosition/pAxis->lIncFactor));
        break;

    case OPMODE_CSV:
        if (pAxis->plPdTargetVelocity)
            EC_SET_FRM_DWORD(pAxis->plPdTargetVelocity, 0);
        break;

    case OPMODE_CST:
        if (pAxis->psPdTargetTorque)
            EC_SET_FRM_WORD(pAxis->psPdTargetTorque, 0);
        break;

    default:
        wErrorID = MC_ERR_INVALID_OPERATION_MODE;
        OsDbgAssert(MC_FALSE);
        break;
    }

    return wErrorID;
}


/********************************************************************************/
/** \brief Trajectory generator: Calculate next movement step
*
* Calculation of pMove->lCommandVel and pMove->lCommandAcc. Written only here!
*/
static EC_T_VOID   MC_TrajNextSetPoint
    (MC_T_MOVEMENT*         pMove)
{
    MC_T_VELACC             lNextVel = 0;
    MC_T_VELACC             lTargetVel = 0;
    MC_T_VELACC             lTargetAcc = 0;
    MC_T_VELACC             lLastAcc = 0;
    MC_T_VELACC             lCurrVel = pMove->lCommandVel;
#ifdef DEBUG
    EC_T_INT                nSegNum = 0;
#endif
    MC_T_BOOL               bResetVelRemainder = MC_FALSE;
    MC_T_VELACC             lAccMax = 0;
    MC_T_VELACC             lVelDiff;

    /* do we have to switch to ramp 2 ? */
    if((pMove->lRemainDist != 0) && (pMove->lRemainDist >= pMove->lSegDist5to7 + pMove->lConstantVel))
    {
        lTargetVel = pMove->lConstantVel;
        pMove->nRampIdx = 0;
    }
    else
    {
        lTargetVel = pMove->lEndVel;
        /* do we have the switchover from ramp 1 to ramp 2 ? */
        if(pMove->nRampIdx == 0)
        {
            pMove->nRampIdx = 1;
            if(pMove->lRamp2AccCorrection != 0)
                pMove->lCommandAcc = pMove->lRamp2AccCorrection;
        }
    }

    pMove->lTargetVel = lTargetVel;

    lVelDiff = lTargetVel - lCurrVel;
    if(lVelDiff > 0)
    {
        pMove->eTrajectState = eTrajStatus_Acc;

        lAccMax = pMove->lRampAccMax[pMove->nRampIdx];
        OsDbgAssert(lAccMax > 0);

        /* calc target acc */
        if(lCurrVel <= lTargetVel - pMove->lVelReduce[pMove->nRampIdx] - pMove->lCommandAcc)
        {
            lTargetAcc = EC_MIN(lTargetVel - lCurrVel, lAccMax);
            /* increase velocity or constant velocity (segments 1-4) */
            if(pMove->lCommandAcc < lTargetAcc)
            {
                /* increase acc  */
                if(pMove->lJerk != 0)
                    pMove->lCommandAcc = EC_MIN(pMove->lCommandAcc + pMove->lJerk, lTargetAcc);
                else
                    pMove->lCommandAcc = lTargetAcc;
                lNextVel = EC_MIN(lCurrVel + pMove->lCommandAcc, lTargetVel);
                lNextVel = EC_MAX(lNextVel, 0);     // no negative velocity
#ifdef DEBUG
                if(pMove->lCommandAcc == lAccMax) nSegNum = 2; else nSegNum = 1;
#endif
            }
            else if(pMove->lCommandAcc == lTargetAcc)       /* constant acc */
            {
                lNextVel = EC_MIN(lCurrVel + pMove->lCommandAcc, lTargetVel);
#ifdef DEBUG
                if(pMove->lCommandAcc == lAccMax) nSegNum = 2; else nSegNum = 3;
#endif
            }
            else
            {   /* decrease acc */
                if(pMove->lJerk != 0)
                    pMove->lCommandAcc = EC_MAX(pMove->lCommandAcc - pMove->lJerk, lTargetAcc);
                else
                    pMove->lCommandAcc = lTargetAcc;
                lNextVel = EC_MIN(lCurrVel + pMove->lCommandAcc, lTargetVel);
#ifdef DEBUG
                if(pMove->lCommandAcc == lAccMax) nSegNum = 2; else nSegNum = 3;
#endif
            }
        }
        else
        {
            /* decrease acc */
            MC_T_VELACC lNextAcc = 0;

            if(pMove->lJerk != 0)
                lNextAcc = pMove->lCommandAcc - pMove->lJerk;
            if(lNextAcc <= 0)
                lNextAcc = EC_MIN(lTargetVel - lCurrVel, lAccMax);

            if((pMove->lVelRemainder[pMove->nRampIdx] != 0) && (lNextAcc <= pMove->lVelRemainder[pMove->nRampIdx]))
            {
                lNextVel = EC_MIN(lCurrVel + pMove->lVelRemainder[pMove->nRampIdx], lTargetVel);
                if(lNextVel == lTargetVel)
                    pMove->lCommandAcc = pMove->lVelRemainder[pMove->nRampIdx];
                pMove->lVelRemainder[pMove->nRampIdx] = 0;
            }
            else
            {
                pMove->lCommandAcc = lNextAcc;
                lNextVel = EC_MIN(lCurrVel + pMove->lCommandAcc, lTargetVel);
            }
#ifdef DEBUG
            if(lNextVel - lCurrVel == lAccMax) nSegNum = 2; else nSegNum = 3;
#endif
        }

#ifdef DEBUG
        if(pMove->nRampIdx == 1) nSegNum += 4;
#endif
        pMove->lCommandVel = lNextVel;
    }
    else if(lVelDiff == 0)
    {
        lNextVel = lTargetVel;
        pMove->eTrajectState = eTrajStatus_Vel;
        pMove->lCommandAcc = 0;
#ifdef DEBUG
        if(!pMove->bEndless) nSegNum = 4;
#endif
    }
    else
    {
        pMove->eTrajectState = eTrajStatus_Dec;
        lAccMax = EC_MIN(pMove->lRampAccMax[pMove->nRampIdx], -1);

        /* calculate remainder package */
        if(pMove->nRampIdx == 1)
        {
            if(!pMove->bRemainStepCalc && !pMove->bEndless && (lTargetVel == pMove->lEndVel))
            {
                pMove->bRemainStepCalc = MC_TRUE;
                pMove->lRemainStep = (pMove->lRemainDist - pMove->lSegDist5to7);
                if(pMove->lRemainStep < 0)
                {
                    OsDbgAssert(pMove->lRemainStep >= 0);
                    pMove->lRemainStep = 0;
                }
            }
        }

        /* don't update lCommandAcc and lCommandVel in case lRemainStep is used in this cycle */
        lLastAcc = pMove->lCommandAcc;

        /* calc target dec */
        if(lCurrVel >= lTargetVel - pMove->lVelReduce[pMove->nRampIdx] - pMove->lCommandAcc)
        {
            lTargetAcc = EC_MAX(lTargetVel - lCurrVel - EC_MAX(0, pMove->lCommandAcc), lAccMax);
            /* increase dec  */
            if(pMove->lCommandAcc > lTargetAcc)
            {
                if(pMove->lJerk != 0)
                    pMove->lCommandAcc = EC_MAX(pMove->lCommandAcc - pMove->lJerk, lTargetAcc);
                else
                    pMove->lCommandAcc = lTargetAcc;

                lNextVel = EC_MAX(lCurrVel + pMove->lCommandAcc, lTargetVel);
#ifdef DEBUG
                if(lNextVel - lCurrVel == lAccMax) nSegNum = 2; else nSegNum = 1;
#endif
            }
            else if(pMove->lCommandAcc == lTargetAcc)       /* constant dec */
            {
                OsDbgAssert(pMove->lCommandAcc != 0);
                lNextVel = EC_MAX(lCurrVel + pMove->lCommandAcc, lTargetVel);
#ifdef DEBUG
                if(lNextVel - lCurrVel == lAccMax) nSegNum = 2; else nSegNum = 3;
#endif
            }
            else
            {
                if(pMove->lJerk != 0)
                    pMove->lCommandAcc = EC_MIN(pMove->lCommandAcc + pMove->lJerk, lTargetAcc);
                else
                    pMove->lCommandAcc = lTargetAcc;

                lNextVel = EC_MAX(lCurrVel + pMove->lCommandAcc, lTargetVel);
#ifdef DEBUG
                if(lNextVel - lCurrVel == lAccMax) nSegNum = 2; else nSegNum = 1;
#endif
            }
        }
        else
        {
            /* decrease dec */
            MC_T_VELACC lNextAcc = 0;

            if(pMove->lJerk != 0)
            {
                lNextAcc = EC_MIN(pMove->lCommandAcc + pMove->lJerk, 0);
            }
            if(lNextAcc >= 0)
                lNextAcc = EC_MAX(lTargetVel - lCurrVel, lAccMax);

            if((pMove->lVelRemainder[pMove->nRampIdx] != 0) && (-lNextAcc <= -pMove->lVelRemainder[pMove->nRampIdx]))
            {
                lNextVel = EC_MAX(lCurrVel + pMove->lVelRemainder[pMove->nRampIdx], lTargetVel);
                bResetVelRemainder = MC_TRUE;
            }
            else
            {
                pMove->lCommandAcc = lNextAcc;
                lNextVel = EC_MAX(lCurrVel + pMove->lCommandAcc, lTargetVel);
            }

#ifdef DEBUG
            if(lNextVel - lCurrVel == lAccMax) nSegNum = 2; else nSegNum = 3;
#endif
        }

#ifdef DEBUG
        if(pMove->nRampIdx == 1) nSegNum += 4;
#endif
        pMove->lCommandVel = lNextVel;

        /* insert remain step ? */
        if(pMove->bRemainStepCalc && (pMove->lRemainStep != 0) && (pMove->lRemainStep >= lNextVel))
        {
            lNextVel = (MC_T_VELACC)pMove->lRemainStep;
            pMove->lRemainStep = 0;
            pMove->lCommandAcc = lLastAcc;
            pMove->lCommandVel = lCurrVel;
#ifdef DEBUG
            nSegNum = 0;
#endif
        }
        else
        {
            if(bResetVelRemainder)
            {
                if(lNextVel == lTargetVel)
                    pMove->lCommandAcc = pMove->lVelRemainder[pMove->nRampIdx];
                pMove->lVelRemainder[pMove->nRampIdx] = 0;
            }
        }
    }

    OsDbgAssert(pMove->lCommandVel >= 0);
    if(pMove->lCommandVel == 0)
    {
        pMove->eTrajectState = eTrajStatus_Idle;
    }

#ifdef DEBUG
    if((nSegNum != 0) && (lNextVel != 0) && (pMove->lRemainDist >= lNextVel))
    {
        pMove->lSegDist[nSegNum-1] -= lNextVel;
        pMove->lSegCycles[nSegNum-1]--;
        OsDbgAssert(pMove->lSegDist[nSegNum-1] >= 0);
        OsDbgAssert(pMove->lSegCycles[nSegNum-1] >= 0);
    }
#endif

    pMove->lTargetPosStep = lNextVel;
    pMove->lTargetAcc     = lTargetAcc;
    pMove->lDoneDist      = pMove->lDoneDist + lNextVel;
    pMove->dwDoneCycles++;
}



/***************************************************************************************************/
/**
\brief  Preview to next velocity package. Don't update content in pMove

* \return next velocity
*/
static MC_T_VELACC   MC_TrajFollowingSetPoint
    (MC_T_MOVEMENT*         pMove)
{
    MC_T_VELACC             lNextVel = 0;
    MC_T_VELACC             lTargetVel = 0;
    MC_T_VELACC             lTargetAcc = 0;
    MC_T_VELACC             lAccMax = 0;
    MC_T_VELACC             lVelDiff;
    MC_T_VELACC             lCurrVel = pMove->lCommandVel;
    MC_T_INT                nRampIdx = pMove->nRampIdx;
    MC_T_VELACC             lCommandAcc = pMove->lCommandAcc;
    MC_T_BOOL               bRemainStepCalc = pMove->bRemainStepCalc;
    MC_T_POS_CALC           lRemainStep = 0;            /* [Inc] Remainder package */

    /* do we have to switch to ramp 2 ? */
    if((pMove->lRemainDist != 0) && (pMove->lRemainDist >= pMove->lSegDist5to7 + pMove->lConstantVel))
    {
        lTargetVel = pMove->lConstantVel;
        nRampIdx = 0;
    }
    else
    {
        lTargetVel = pMove->lEndVel;
        /* do we have the switchover from ramp 1 to ramp 2 ? */
        if(nRampIdx == 0)
        {
            nRampIdx = 1;
            if(pMove->lRamp2AccCorrection != 0)
                lCommandAcc = pMove->lRamp2AccCorrection;
        }
    }

    lVelDiff = lTargetVel - lCurrVel;
    if(lVelDiff > 0)
    {
        lAccMax = pMove->lRampAccMax[nRampIdx];

        /* calc target acc */
        if(lCurrVel <= lTargetVel - pMove->lVelReduce[nRampIdx] - lCommandAcc)
        {
            lTargetAcc = EC_MIN(lTargetVel - lCurrVel, lAccMax);
            /* increase velocity or constant velocity (segments 1-4) */
            if(lCommandAcc < lTargetAcc)
            {
                /* increase acc  */
                if(pMove->lJerk != 0)
                    lCommandAcc = EC_MIN(lCommandAcc + pMove->lJerk, lTargetAcc);
                else
                    lCommandAcc = lTargetAcc;
                lNextVel = EC_MIN(lCurrVel + lCommandAcc, lTargetVel);
                lNextVel = EC_MAX(lNextVel, 0);     // no negative velocity
            }
            else if(lCommandAcc == lTargetAcc)       /* constant acc */
            {
                lNextVel = EC_MIN(lCurrVel + lCommandAcc, lTargetVel);
            }
            else
            {   /* decrease acc */
                if(pMove->lJerk != 0)
                    lCommandAcc = EC_MAX(lCommandAcc - pMove->lJerk, lTargetAcc);
                else
                    lCommandAcc = lTargetAcc;
                lNextVel = EC_MIN(lCurrVel + lCommandAcc, lTargetVel);
            }
        }
        else
        {
            /* decrease acc */
            MC_T_VELACC lNextAcc = 0;

            if(pMove->lJerk != 0)
                lNextAcc = lCommandAcc - pMove->lJerk;
            if(lNextAcc <= 0)
                lNextAcc = EC_MIN(lTargetVel - lCurrVel, lAccMax);

            if((pMove->lVelRemainder[nRampIdx] != 0) && (lNextAcc <= pMove->lVelRemainder[nRampIdx]))
            {
                lNextVel = EC_MIN(lCurrVel + pMove->lVelRemainder[nRampIdx], lTargetVel);
                if(lNextVel == lTargetVel)
                    lCommandAcc = pMove->lVelRemainder[nRampIdx];
            }
            else
            {
                lNextVel = EC_MIN(lCurrVel + lNextAcc, lTargetVel);
            }
        }
    }
    else if(lVelDiff == 0)
    {
        lNextVel = lTargetVel;
    }
    else
    {
        lAccMax = EC_MIN(pMove->lRampAccMax[nRampIdx], -1);

        /* calculate remainder package */
        if(nRampIdx == 1)
        {
            if(!pMove->bRemainStepCalc && !pMove->bEndless && (lTargetVel == pMove->lEndVel))
            {
                bRemainStepCalc = MC_TRUE;
                lRemainStep = (pMove->lRemainDist - pMove->lSegDist5to7);
                if(lRemainStep < 0) lRemainStep = 0;
            }
        }

        /* calc target dec */
        if(lCurrVel >= lTargetVel - pMove->lVelReduce[nRampIdx] - lCommandAcc)
        {
            lTargetAcc = EC_MAX(lTargetVel - lCurrVel - EC_MAX(0, lCommandAcc), lAccMax);
            /* increase dec  */
            if(lCommandAcc > lTargetAcc)
            {
                if(pMove->lJerk != 0)
                    lCommandAcc = EC_MAX(lCommandAcc - pMove->lJerk, lTargetAcc);
                else
                    lCommandAcc = lTargetAcc;

                lNextVel = EC_MAX(lCurrVel + lCommandAcc, lTargetVel);
            }
            else if(lCommandAcc == lTargetAcc)       /* constant dec */
            {
                lNextVel = EC_MAX(lCurrVel + lCommandAcc, lTargetVel);
            }
            else
            {
                if(pMove->lJerk != 0)
                    lCommandAcc = EC_MIN(lCommandAcc + pMove->lJerk, lTargetAcc);
                else
                    lCommandAcc = lTargetAcc;

                lNextVel = EC_MAX(lCurrVel + lCommandAcc, lTargetVel);
            }
        }
        else
        {
            /* decrease dec */
            MC_T_VELACC lNextAcc = 0;

            if(pMove->lJerk != 0)
            {
                lNextAcc = EC_MIN(lCommandAcc + pMove->lJerk, 0);
            }
            if(lNextAcc >= 0)
                lNextAcc = EC_MAX(lTargetVel - lCurrVel, lAccMax);

            if((pMove->lVelRemainder[nRampIdx] != 0) && (-lNextAcc <= -pMove->lVelRemainder[nRampIdx]))
            {
                lNextVel = EC_MAX(lCurrVel + pMove->lVelRemainder[nRampIdx], lTargetVel);
            }
            else
            {
                lNextVel = EC_MAX(lCurrVel + lNextAcc, lTargetVel);
            }
        }

        /* insert remain step ? */
        if(bRemainStepCalc && (lRemainStep != 0) && (pMove->lRemainStep >= lNextVel))
        {
            lNextVel = (MC_T_VELACC)lRemainStep;
        }
    }

    return lNextVel;
}


/***************************************************************************************************/
/**
\brief  Abort Running MCFB.

* \return N/A
*/
static EC_T_VOID   MC_AbortCommand
    (MC_T_AXIS_REF*     pAxis
     ,EC_T_WORD         wNewFbID)
{
   if (pAxis->pWorkFb == EC_NULL || pAxis->pWorkFb->pFB == EC_NULL) return; // No FB buffered

   switch(pAxis->pWorkFb->pFB->wFbID)
   {
   case MCFB_ID_HOME:
   {
       MC_HOME_T*    pMcfb = (MC_HOME_T*)pAxis->pWorkFb->pFB;
       if (pMcfb->bActive)
       {
           pMcfb->bBusy = MC_FALSE;
           pMcfb->bActive = MC_FALSE;
           pMcfb->bCommandAborted  = MC_TRUE;
           pMcfb->ChangeState(FBS_COMMAND_ABORTED);

           EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_Halt        : Exe=%d Busy=%d Active=%d Done=%d CmdAborted=%d Error=%d\n",
              pMcfb->bExecute, pMcfb->bBusy, pMcfb->bActive, pMcfb->bDone, pMcfb->bCommandAborted, pMcfb->bError));
       }
   }
   break;

   case MCFB_ID_HALT:
      {
         MC_HALT_T*    pMcfb = (MC_HALT_T*)pAxis->pWorkFb->pFB;
         if(pMcfb->bActive)
         {
            pMcfb->bBusy = MC_FALSE;
            pMcfb->bActive = MC_FALSE;
            pMcfb->bCommandAborted  = MC_TRUE;
            pMcfb->ChangeState(FBS_COMMAND_ABORTED);

            EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_Halt        : Exe=%d Busy=%d Active=%d Done=%d CmdAborted=%d Error=%d\n",
               pMcfb->bExecute, pMcfb->bBusy, pMcfb->bActive, pMcfb->bDone, pMcfb->bCommandAborted, pMcfb->bError));
         }
      }
      break;

   case MCFB_ID_MOVE_ABSOLUTE:
      {
         MC_MOVE_ABSOLUTE_T*    pMcfb = (MC_MOVE_ABSOLUTE_T*)pAxis->pWorkFb->pFB;
         if(pMcfb->bActive)
         {
            /* stop current movement */
            pAxis->oMove.lRemainDist = EC_MIN(pAxis->oMove.lRemainDist, pAxis->oMove.lSegDist5to7);
            pMcfb->bBusy = MC_FALSE;
            pMcfb->bActive = MC_FALSE;
            pMcfb->bCommandAborted = MC_TRUE;
            pMcfb->ChangeState(FBS_COMMAND_ABORTED);

            EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_AbortCommand: MoveAbsolute-ActiveFbID=%d aborted by NewFbID=%d\n", pAxis->pWorkFb->pFB->wFbID, wNewFbID));
            EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_MoveAbsolute: Exe=%d Busy=%d Active=%d Done=%d CmdAborted=%d Error=%d\n",
               pMcfb->bExecute, pMcfb->bBusy, pMcfb->bActive, pMcfb->bDone, pMcfb->bCommandAborted, pMcfb->bError));
         }
      }
      break;
   case MCFB_ID_MOVE_RELATIVE:
      {
         MC_MOVE_RELATIVE_T*    pMcfb = (MC_MOVE_RELATIVE_T*)pAxis->pWorkFb->pFB;
         if(pMcfb->bActive)
         {
            /* stop current movement */
            pAxis->oMove.lRemainDist = EC_MIN(pAxis->oMove.lRemainDist, pAxis->oMove.lSegDist5to7);
            pMcfb->bBusy = MC_FALSE;
            pMcfb->bActive = MC_FALSE;
            pMcfb->bCommandAborted = MC_TRUE;
            pMcfb->ChangeState(FBS_COMMAND_ABORTED);

            EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_AbortCommand: MoveRelative-ActiveFbID=%d aborted by NewFbID=%d\n", pAxis->pWorkFb->pFB->wFbID, wNewFbID));
            EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_MoveRelative: Exe=%d Busy=%d Active=%d Done=%d CmdAborted=%d Error=%d\n",
               pMcfb->bExecute, pMcfb->bBusy, pMcfb->bActive, pMcfb->bDone, pMcfb->bCommandAborted, pMcfb->bError));
         }
      }
      break;
   case MCFB_ID_MOVE_VELOCITY:
      {
         MC_MOVE_VELOCITY_T*    pMcfb = (MC_MOVE_VELOCITY_T*)pAxis->pWorkFb->pFB;
         if(pMcfb->bActive)
         {
            pMcfb->bInVelocity = MC_FALSE;
            pMcfb->bBusy = MC_FALSE;
            pMcfb->bActive = MC_FALSE;
            pMcfb->bCommandAborted = MC_TRUE;
            pMcfb->ChangeState(FBS_COMMAND_ABORTED);

            EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_MoveVelocity: Exe=%d Busy=%d Active=%d InVelocity=%d CmdAborted=%d Error=%d\n",
                pMcfb->bExecute, pMcfb->bBusy, pMcfb->bActive, pMcfb->bInVelocity, pMcfb->bCommandAborted, pMcfb->bError));
         }
      }
      break;
   case MCFB_ID_MOVE_CONTINUOUS_ABSOLUTE:
      {
         MC_MOVE_CONT_ABSOLUTE_T*    pMcfb = (MC_MOVE_CONT_ABSOLUTE_T*)pAxis->pWorkFb->pFB;
         if(pMcfb->bActive)
         {
            /* stop current movement */
            pAxis->oMove.lRemainDist = EC_MIN(pAxis->oMove.lRemainDist, pAxis->oMove.lSegDist5to7);
            pMcfb->bBusy = MC_FALSE;
            pMcfb->bActive = MC_FALSE;
            pMcfb->bCommandAborted = MC_TRUE;
            pMcfb->ChangeState(FBS_COMMAND_ABORTED);

            EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_AbortCommand: MoveAbsolute-ActiveFbID=%d aborted by NewFbID=%d\n", pAxis->pWorkFb->pFB->wFbID, wNewFbID));
            EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_MoveContAbsolute: Exe=%d Busy=%d Active=%d InEndVelocity=%d CmdAborted=%d Error=%d\n",
               pMcfb->bExecute, pMcfb->bBusy, pMcfb->bActive, pMcfb->bInEndVelocity, pMcfb->bCommandAborted, pMcfb->bError));
         }
      }
      break;
   case MCFB_ID_MOVE_CONTINUOUS_RELATIVE:
      {
         MC_MOVE_CONT_RELATIVE_T*    pMcfb = (MC_MOVE_CONT_RELATIVE_T*)pAxis->pWorkFb->pFB;
         if(pMcfb->bActive)
         {
            /* stop current movement */
            pAxis->oMove.lRemainDist = EC_MIN(pAxis->oMove.lRemainDist, pAxis->oMove.lSegDist5to7);
            pMcfb->bBusy = MC_FALSE;
            pMcfb->bActive = MC_FALSE;
            pMcfb->bCommandAborted = MC_TRUE;
            pMcfb->ChangeState(FBS_COMMAND_ABORTED);

           EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_AbortCommand: MoveRelative-ActiveFbID=%d aborted by NewFbID=%d\n", pAxis->pWorkFb->pFB->wFbID, wNewFbID));
           EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_MoveContRelative: Exe=%d Busy=%d Active=%d InEndVelocity=%d CmdAborted=%d Error=%d\n",
              pMcfb->bExecute, pMcfb->bBusy, pMcfb->bActive, pMcfb->bInEndVelocity, pMcfb->bCommandAborted, pMcfb->bError));
         }
      }
      break;

   case MCFB_ID_POSITION_PROFILE:
      {
#ifdef EC_MOTION_SUPPORT_MC_POSITION_PROFILE
         MC_POSITION_PROFILE_T*    pMcfb = (MC_POSITION_PROFILE_T*)pAxis->pWorkFb->pFB;
         if(pMcfb->bActive)
         {
            pMcfb->bBusy = MC_FALSE;
            pMcfb->bActive = MC_FALSE;
            pMcfb->bCommandAborted  = MC_TRUE;
            pMcfb->ChangeState(FBS_COMMAND_ABORTED);

           EcLogMsg(EC_LOG_LEVEL_INFO, (pEcLogContext, EC_LOG_LEVEL_INFO, "MC_PositionProfile        : Exe=%d Busy=%d Active=%d Done=%d CmdAborted=%d Error=%d\n",
              pMcfb->bExecute, pMcfb->bBusy, pMcfb->bActive, pMcfb->bDone, pMcfb->bCommandAborted, pMcfb->bError));
         }
#endif
      }
      break;

   case MCFB_ID_CAM_IN:
       break;

   case MCFB_ID_GEAR_IN:
        break;

   default:
      OsDbgAssert(MC_FALSE);
      break;
   }
}


/********************************************************************************/
/** \brief Check software limits and calculate maximum distance
*
* \return: error code
*/
static EC_T_WORD   MC_CheckSoftwareLimits
    (MC_T_AXIS_REF*     pAxis
    ,MC_T_POS_CALC      lDistance
    ,MC_T_POS_CALC*     pMaxDistance)
{
    EC_T_WORD           wErrorID = MC_NO_ERROR;
    MC_T_POS_CALC       lTargetPos = 0;

    if (pAxis->dwDirection == MC_DIR_POSITIVE)
    {
        lTargetPos = pAxis->lCommandedPosition + lDistance;
    }
    else if (pAxis->dwDirection == MC_DIR_NEGATIVE)
    {
        lTargetPos = pAxis->lCommandedPosition - lDistance;
    }

    if (pAxis->bSWLimitPositiveEna)      /* positive limit enabled ? */
    {
        if (lTargetPos > pAxis->lSWLimitPositivePos)
        {
            wErrorID = MC_ERR_SW_LIMIT_POS;
            if (pMaxDistance != EC_NULL)
            {
                *pMaxDistance = pAxis->lSWLimitPositivePos - pAxis->lCommandedPosition - 1;
            }
        }
    }

    if (pAxis->bSWLimitNegativeEna)      /* negative limit enabled ? */
    {
        if (lTargetPos < pAxis->lSWLimitNegativePos)
        {
            wErrorID = MC_ERR_SW_LIMIT_NEG;
            if (pMaxDistance != EC_NULL)
            {
                *pMaxDistance = pAxis->lCommandedPosition - pAxis->lSWLimitNegativePos - 1;
            }
        }
    }

    return wErrorID;
}


/* clear FB buffer */
static EC_T_VOID   MC_BufferClear
    (MC_T_AXIS_REF*         pAxis)
{
    pAxis->nFbFillIdx  = 0;
    pAxis->nFbEmptyIdx = 0;
    pAxis->pWorkFb = pAxis->apFBBuffer;
    OsMemset(pAxis->apFBBuffer, 0, sizeof(pAxis->apFBBuffer));
}

/* Returns true if next FB is activated, false otherwise */
static EC_T_WORD MC_BufferActivateNext(
     MC_T_AXIS_REF *pAxis, MC_BUFFERED_FB_T *pMcfb, MC_T_BOOL *fbActive)
{
    *fbActive = false;

    /* in case of an axis error we can't get active anymore with this FB */
    if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_ERROR_STOP)
       return MC_ERR_INVALID_PLCOPEN_STATE;

    if (pAxis->pWorkFb == EC_NULL || pAxis->pWorkFb->pFB == EC_NULL)
       return MC_NO_ERROR;

   /* \sa table 4. 'Done AND NOT Execute' condition.
    * MC_PLCOPEN_STATE_STAND_STILL is set if Execute = 0 and Done = 1
    */
   if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STOPPING)
      return MC_NO_ERROR;

   if (pAxis->pWorkFb->pFB->bActive)
      return MC_NO_ERROR;  /* something else active ? */

   /* Are we next? */
   if (pAxis->apFBBuffer[pAxis->nFbEmptyIdx].pFB != pMcfb)
      return MC_NO_ERROR;

   /* remove FB from buffer */
   pAxis->pWorkFb = &pAxis->apFBBuffer[pAxis->nFbEmptyIdx];
   pAxis->nFbEmptyIdx = (pAxis->nFbEmptyIdx + 1) % MC_FB_BUFFER_LEN;
   *fbActive = MC_TRUE;

   return MC_NO_ERROR;
}

/* put FB into buffer */
static EC_T_WORD   MC_BufferPut
    (MC_T_AXIS_REF*         pAxis
    ,MC_BUFFERED_FB_T*      pMcfb
    ,MC_T_REAL              fEndVel
    ,MC_T_BUFFERENTRY**     ppPrevFb)
{
    EC_T_WORD           wErrorID = MC_NO_ERROR;
    EC_T_INT            nBufCount = (pAxis->nFbFillIdx + MC_FB_BUFFER_LEN - pAxis->nFbEmptyIdx) % MC_FB_BUFFER_LEN;

    if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STOPPING && pMcfb->dwBufferMode == MC_ABORTING)
    {
       /* 3.3 MC_Stop. The axis rejects motion commands as long as MC_Stop parameter
        * Execute = TRUE, FB reports an error indicating the busy MC_Stop command.
        */
       return MC_ERR_BUSY_STOPPING;
    }

    if(ppPrevFb != EC_NULL)
        *ppPrevFb = EC_NULL;
    if(nBufCount < MC_FB_BUFFER_LEN-1)
    {
        MC_T_BUFFERENTRY*   pMyEntry = &pAxis->apFBBuffer[pAxis->nFbFillIdx];

        pMyEntry->pFB       = pMcfb;
        pMyEntry->fEndVel   = fEndVel;
        if(ppPrevFb != EC_NULL)
            *ppPrevFb  = pMyEntry->pPrev;
        pAxis->nFbFillIdx   = (pAxis->nFbFillIdx + 1) % MC_FB_BUFFER_LEN;
        pAxis->apFBBuffer[pAxis->nFbFillIdx].pPrev = pMyEntry;
    }
    else
    {
        OsDbgAssert(MC_FALSE);
        wErrorID = MC_ERR_BUFFER_OVERFLOW;
        EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR in MC_BufferPut(): FB buffer is full. Maximum capacity is = %d", MC_FB_BUFFER_LEN-1));
    }

    return wErrorID;
}


/***************************************************************************************************/
/**
* \brief    Sets actual position
*
* \param    MC_T_AXIS_REF*  pAxis       Pointer to an axis structure
* \param    MC_T_REAL       fPosition   Position to set, [mm]
* \param    MC_T_BOOL       bRelative   EC_FALSE = the new position is absolute
*
* \return   -
*/
static EC_T_VOID MC_SetPosition(
    MC_T_AXIS_REF*      pAxis,      /**< [in/out]   Pointer to an axis structure */
    MC_T_REAL           fPosition,  /**< [in]       Position to set, [mm] */
    MC_T_BOOL           bRelative   /**< [in]       EC_FALSE = the new position is absolute */
    )
{
    MC_T_POS_CALC   lHelpPos = 0;
    MC_T_POS_CALC   lCommandedPos = 0;

    lHelpPos = (MC_T_POS_CALC)(fPosition * (MC_T_POS_CALC)pAxis->dwCalcIncPerMM);
    lCommandedPos = pAxis->lCommandedPosition - pAxis->lPosOffset;
    if (bRelative)
    {
        pAxis->lPosOffset = pAxis->lPosOffset + lHelpPos;
    }
    else
    {
        pAxis->lPosOffset = lHelpPos - pAxis->lActualPosRaw;
    }

    /* update command position */
    pAxis->lCommandedPosition = lCommandedPos + pAxis->lPosOffset;

    /* update actual position */
    pAxis->lActualPosition = pAxis->lOldActualPosition = pAxis->lActualPosRaw + pAxis->lPosOffset;

    return;
}

/***************************************************************************************************/
/**
\brief  Error Text for given error number
\return Error Text.
*/
const EC_T_CHAR* MC_GetErrorText(
    EC_T_WORD wErrorID /**< [in]   Error number */
)
{
    const EC_T_CHAR*  pszText     = EC_NULL;

    switch( wErrorID )
    {
    case MC_NO_ERROR:                           pszText = MC_SZTXT_NO_ERROR; break;
    case MC_ERR_INVALID_PARAMETER:              pszText = MC_SZTXT_ERR_INVALID_PARAMETER; break;
    case MC_ERR_INVALID_OPERATION_MODE:         pszText = MC_SZTXT_ERR_INVALID_OPERATION_MODE; break;
    case MC_ERR_INVALID_PLCOPEN_STATE:          pszText = MC_SZTXT_ERR_INVALID_PLCOPEN_STATE; break;
    case MC_ERR_FOLLOWING:                      pszText = MC_SZTXT_ERR_FOLLOWING; break;
    case MC_ERR_INVALID_STATUS_WORD:            pszText = MC_SZTXT_ERR_INVALID_STATUS_WORD; break;
    case MC_ERR_AXIS_FAULT:                     pszText = MC_SZTXT_ERR_AXIS_FAULT; break;
    case MC_ERR_BUFFER_OVERFLOW:                pszText = MC_SZTXT_ERR_BUFFER_OVERFLOW; break;
    case MC_ERR_PARA_NOT_SUPPORTED:             pszText = MC_SZTXT_ERR_PARA_NOT_SUPPORTED; break;
    case MC_ERR_SW_LIMIT_POS:                   pszText = MC_SZTXT_ERR_SW_LIMIT_POS; break;
    case MC_ERR_SW_LIMIT_NEG:                   pszText = MC_SZTXT_ERR_SW_LIMIT_NEG; break;

    case MC_ERR_DIST_OUT_OF_RANGE:              pszText = MC_SZTXT_ERR_DIST_OUT_OF_RANGE; break;
    case MC_ERR_VEL_OUT_OF_RANGE:               pszText = MC_SZTXT_ERR_VEL_OUT_OF_RANGE; break;
    case MC_ERR_ACC_OUT_OF_RANGE:               pszText = MC_SZTXT_ERR_ACC_OUT_OF_RANGE; break;
    case MC_ERR_DEC_OUT_OF_RANGE:               pszText = MC_SZTXT_ERR_DEC_OUT_OF_RANGE; break;
    case MC_ERR_JERK_OUT_OF_RANGE:              pszText = MC_SZTXT_ERR_JERK_OUT_OF_RANGE; break;

    case MC_ERR_CALC_JERK_ACCDEC_MIN:           pszText = MC_SZTXT_ERR_CALC_JERK_ACCDEC_MIN; break;
    case MC_ERR_CALC_JERK_TOMANY_CYC:           pszText = MC_SZTXT_ERR_CALC_JERK_TOMANY_CYC; break;
    case MC_ERR_OVERFLOW:                       pszText = MC_SZTXT_ERR_OVERFLOW; break;
    case MC_ERR_BUFFERMODE_NOT_SUPP:            pszText = MC_SZTXT_ERR_BUFFERMODE_NOT_SUPP; break;
    case MC_ERR_CALC_DIST_NOT_MATCHING:         pszText = MC_SZTXT_ERR_CALC_DIST_NOT_MATCHING; break;
    case MC_ERR_CALC_VEL_NOT_MATCHING:          pszText = MC_SZTXT_ERR_CALC_VEL_NOT_MATCHING; break;
    case MC_ERR_DIR_CHANGED:                    pszText = MC_SZTXT_ERR_DIR_CHANGED; break;
    case MC_ERR_CONTUPDATE_DIR_CHANGED:         pszText = MC_SZTXT_ERR_CONTUPDATE_DIR_CHANGED; break;
    case MC_ERR_CONTUPDATE_DIST_CHANGED:        pszText = MC_SZTXT_ERR_CONTUPDATE_DIST_CHANGED; break;

    case MC_ERR_CALC_CONSTANT_VEL_TO_HIGH:      pszText = MC_SZTXT_ERR_CALC_CONSTANT_VEL_TO_HIGH; break;
    case MC_ERR_CALC_CONSTANT_VEL_TO_LOW:       pszText = MC_SZTXT_ERR_CALC_CONSTANT_VEL_TO_LOW; break;
    case MC_ERR_CALC_END_VEL_TO_HIGH:           pszText = MC_SZTXT_ERR_CALC_END_VEL_TO_HIGH; break;
    case MC_ERR_CALC_END_VEL_TO_LOW:            pszText = MC_SZTXT_ERR_CALC_END_VEL_TO_LOW; break;
    case MC_ERR_CALC_RAMPS_TO_BIG:              pszText = MC_SZTXT_ERR_CALC_RAMPS_TO_BIG; break;
    case MC_ERR_BUSY_STOPPING:                  pszText = MC_SZTXT_ERR_BUSY_STOPPING; break;

    case MC_ERR_INVALID_INPUT_NUM:              pszText = MC_SZTXT_ERR_INVALID_INPUT_NUM; break;
    case MC_ERR_NO_INPUT_DATA_PTR:              pszText = MC_SZTXT_ERR_NO_INPUT_DATA_PTR; break;
    case MC_ERR_INVALID_OUTPUT_NUM:             pszText = MC_SZTXT_ERR_INVALID_OUTPUT_NUM; break;
    case MC_ERR_NO_OUTPUT_DATA_PTR:             pszText = MC_SZTXT_ERR_NO_OUTPUT_DATA_PTR; break;
    case MC_ERR_NO_ECAT_PROFILE:                pszText = MC_SZTXT_ERR_NO_ECAT_PROFILE; break;

    case MC_ERR_EXTENDED:                       pszText = MC_SZTXT_ERR_EXTENDED; break;
    case MC_ERR_HOMING:                         pszText = MC_SZTXT_ERR_HOMING; break;

#ifdef EC_MOTION_SUPPORT_MC_CAMMING
    case MC_ERR_MASTER_AXIS_INVALID:            pszText = MC_SZTXT_ERR_MASTER_AXIS_INVALID; break;
    case MC_ERR_CAM_TABLE_ID_INVALID:           pszText = MC_SZTXT_ERR_CAM_TABLE_ID_INVALID; break;
    case MC_ERR_CAM_SPLINE_INVALID:             pszText = MC_SZTXT_ERR_CAM_SPLINE_INVALID; break;
    case MC_ERR_INVALID_PLCOPEN_STATE_SLAVE:    pszText = MC_SZTXT_ERR_INVALID_PLCOPEN_STATE_SLAVE; break;
    case MC_ERR_START_MODE_INVALID:             pszText = MC_SZTXT_ERR_START_MODE_INVALID; break;
    case MC_ERR_CAM_TABLE_ELEM_TOO_LESS:        pszText = MC_SZTXT_ERR_CAM_TABLE_ELEM_TOO_LESS; break;
    case MC_ERR_CAM_TABLE_ELEM_TOO_MANY:        pszText = MC_SZTXT_ERR_CAM_TABLE_ELEM_TOO_MANY; break;
    case MC_ERR_CAM_TABLE_TYPE_INVALID:         pszText = MC_SZTXT_ERR_CAM_TABLE_TYPE_INVALID; break;
    case MC_ERR_CAM_TABLE_VARTYPE_INVALID:      pszText = MC_SZTXT_ERR_CAM_TABLE_VARTYPE_INVALID; break;
    case MC_ERR_CAM_TABLE_INTERPOL_INVALID:     pszText = MC_SZTXT_ERR_CAM_TABLE_INTERPOL_INVALID; break;
    case MC_ERR_CAM_TABLE_DATA_INVALID:         pszText = MC_SZTXT_ERR_CAM_TABLE_DATA_INVALID; break;
    case MC_ERR_CAM_IN_MASPOS_TOO_SMALL:        pszText = MC_SZTXT_ERR_CAM_IN_MASPOS_TOO_SMALL; break;
    case MC_ERR_CAM_IN_MASPOS_TOO_BIG:          pszText = MC_SZTXT_ERR_CAM_IN_MASPOS_TOO_BIG; break;
    case MC_ERR_CAM_IN_MAS_PERIOD_ZERO:         pszText = MC_SZTXT_ERR_CAM_IN_MAS_PERIOD_ZERO;break;
    case MC_ERR_CAM_IN_MAS_PERIOD_NEGA:         pszText = MC_SZTXT_ERR_CAM_IN_MAS_PERIOD_NEGA;break;
    case MC_ERR_CAM_IN_MASPOS_NULL_MISSING:     pszText = MC_SZTXT_ERR_CAM_IN_MASPOS_NULL_MISSING;break;
#endif
#ifdef EC_MOTION_SUPPORT_MC_GEARING
    case MC_ERR_GEAR_PARA_INVALID:          pszText = MC_SZTXT_ERR_GEAR_PARA_INVALID; break;
    case MC_ERR_GEAR_INVALID_MAS_STATE:     pszText = MC_SZTXT_ERR_GEAR_INVALID_MAS_STATE; break;
#endif
    default:
        pszText = "Unknown Error Text";
        break;
    }
    return pszText;
}


/********************************************************************************************/
/**
 * \brief  Get log message header. Has to match with values in MC_GetLogMsg()
 *
 * \return -
 */
EC_T_DWORD MC_GetLogMsgHeader(
    EC_T_CHAR* pszLog,
    EC_T_INT   nMaxSize
)
{
    if (pszLog != EC_NULL)
    {
        OsSnprintf(pszLog, nMaxSize, "time[ms];CycCnt;Act.Pos [Inc];Act. Vel [Inc];Comm.Pos [Inc];Cont Err; Traj.State; Step [Inc]; \n");
    }

    return EC_E_NOERROR;
}


#ifdef EC_MOTION_SUPPORT_PP_MODE
/********************************************************************************************/
/**
\brief  Manages a state machine for profile position mode

\return -
*/
static EC_T_VOID MC_PPSm(MC_T_AXIS_REF* pAxis)
{
    MC_T_MOVEMENT*  pMove = &pAxis->oMove;
    EC_T_SDWORD     sdwCurrTargetPosition = 0;

    if (pAxis->eOperationMode == OPMODE_HOMING)
    {
        switch (pMove->eProfileState)
        {
        case eProfileStatus_Idle:
            break;

        case eProfileStatus_Error:
            // The state machine stays in this state until
            // it will be switched into another state
            break;

        case eProfileStatus_SwitchToHomingMode:
            // Store the actual mode
            if( pAxis->wActOpMode != DRV_MODE_OP_HOMING)
                pAxis->wActOpMode = pAxis->wProfileOperationMode;
            // Switch into homing mode
            pAxis->wProfileOperationMode = DRV_MODE_OP_HOMING;

            // Reset homing command, in order to issue rising edge afterwards
            pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & (~DRV_CTRL_START_HOMING));

            // Goto waiting for acknowledge
            pMove->eProfileState = eProfileStatus_StartHoming;

            break;

        case eProfileStatus_StartHoming:
            // Wait until controller accept mode switch
            if (*pAxis->pbyPdModeOfOperationDisplay == DRV_MODE_OP_HOMING)
            {
                // Take back halt
                pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~(DRV_CRTL_HALT));
                // Issue a start homing commando
                pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord | DRV_CTRL_START_HOMING);

                // Start homing sequnce
                pMove->eProfileState = eProfileStatus_WaitHomingDone;
            }
            else
            {
                // Stay in this state
            }
            break;

        case eProfileStatus_WaitHomingDone:
            if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STOPPING)
            {
                // Stop command was issued during movement
                // Issue a quick stop
                pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~DRV_CTRL_CMD_QUICK_STOP);
                // Take back start movement commando
                pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~DRV_CTRL_START_HOMING);

                // Switch to waiting for stop end
                pMove->eProfileState = eProfileStatus_WaitQStop;
            }
            // Wait until sequnece ends
            if ((pAxis->wStatusWord & DRV_STAT_HOMING_ATTAINED) && (pAxis->wStatusWord & DRV_STAT_TARGET_REACHED))
            {
                // Take back start homing commando
                pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & (~DRV_CTRL_START_HOMING));

                // Shift commanded position to use with succeeding command
                pAxis->lCommandedPosition = pAxis->lActualPosition;
                // Inevitably set target position to use later on by movement FBs
                EC_SET_FRM_DWORD(pAxis->plPdTargetPosition, EC_GET_FRM_DWORD(pAxis->plPdActualPosition));

                // Return to previous mode
                pAxis->wProfileOperationMode = pAxis->wActOpMode;
                pAxis->eOperationMode = OPMODE_PP;

                // Switch to idle state
                pMove->eProfileState = eProfileStatus_Idle;
            }
            // Check for errors
            if (pAxis->wStatusWord & DRV_STAT_HOMING_ERROR)
            {
                // Take back start homing commando
                pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & (~DRV_CTRL_START_HOMING));

                // Return to previous mode
                pAxis->wProfileOperationMode = pAxis->wActOpMode;
                pAxis->eOperationMode = OPMODE_PP;

                // Switch to error state
                pMove->eProfileState = eProfileStatus_Error;
            }
            break;

        case eProfileStatus_WaitQStop:
            // Wait until quick stop ends
            if (pAxis->wStatusWord & DRV_STAT_SWITCH_ON_DIS)
            {
                // Return to previous mode
                pAxis->wProfileOperationMode = pAxis->wActOpMode;
                pAxis->eOperationMode = OPMODE_PP;

                // Switch to idle state
                pMove->eProfileState = eProfileStatus_Idle;
            }
            else
            {
                // Stay in this state
            }
            break;

        case eProfileStatus_Halt:
            // Command HALT
            pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord | DRV_CRTL_HALT);

            // Switch to wait state
            pMove->eProfileState = eProfileStatus_WaitHaltDone;
            break;

        case eProfileStatus_WaitHaltDone:
            // Wait until movement ends
            if (pAxis->wStatusWord & DRV_STAT_VELOCITY_ZERO)
            {
                // Return to previous mode
                pAxis->wProfileOperationMode = pAxis->wActOpMode;
                pAxis->eOperationMode = OPMODE_PP;

                // Switch to idle state
                pMove->eProfileState = eProfileStatus_Idle;
            }
            else
            {
                // Stay in this state
            }
            break;

        default:
            OsDbgAssert(EC_FALSE);
            break;
        }
    }
    else
    {
        if (pAxis->eOperationMode == OPMODE_PP)
        {
            switch (pMove->eProfileState)
            {
            case eProfileStatus_Idle:
                break;

            case eProfileStatus_Error:
                // The state machine stays in this state until
                // it will be switched into another state
                break;

            case eProfileStatus_NewSetPoint:
                // Set profile velocity
                if ((pAxis->pbyPdModeOfOperationDisplay) != EC_NULL && (pAxis->pbyPdModeOfOperation != EC_NULL))
                {
                    if (*pAxis->pbyPdModeOfOperationDisplay != pAxis->wProfileOperationMode)
                    {
                        break;
                    }
                }
                // Set profile accelaration
                if (pAxis->pdwPdProfileAcc != EC_NULL) EC_SET_FRM_DWORD(pAxis->pdwPdProfileAcc, pMove->lMaxAcc);
                // Set profile deccelaration
                if (pAxis->pdwPdProfileDec != EC_NULL) EC_SET_FRM_DWORD(pAxis->pdwPdProfileDec, pMove->lMaxDec);

                /* set target position */
                sdwCurrTargetPosition = EC_GET_FRM_DWORD(pAxis->plPdTargetPosition);

                if (pAxis->dwDirection == MC_DIR_POSITIVE)
                {
                    sdwCurrTargetPosition = (EC_T_SDWORD)(sdwCurrTargetPosition + pMove->lTotalDist / pAxis->lIncFactor);
                }
                else
                {
                    sdwCurrTargetPosition = (EC_T_SDWORD)(sdwCurrTargetPosition - pMove->lTotalDist / pAxis->lIncFactor);
                }

                pAxis->lCommandedPosition = (sdwCurrTargetPosition + pAxis->lPosOffset) * pAxis->lIncFactor;

                if (pAxis->wProfileOperationMode == DRV_MODE_OP_PROF_POS)
                {
                    // Set profile velocity
                    if (pAxis->pdwPdProfileVelocity != EC_NULL) EC_SET_FRM_DWORD(pAxis->pdwPdProfileVelocity, pMove->lConstantVel);
                    EC_SET_FRM_DWORD(pAxis->plPdTargetPosition, sdwCurrTargetPosition);

                    // Issue a start movement commando
                    pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord | DRV_CTRL_PP_NEW_SET_POINT);
                }
                else if (pAxis->wProfileOperationMode == DRV_MODE_OP_PROF_VEL)
                {
                    if (pAxis->dwDirection == MC_DIR_POSITIVE)
                    {
                        if (pAxis->plPdTargetVelocity != EC_NULL) EC_SET_FRM_DWORD(pAxis->plPdTargetVelocity, pMove->lConstantVel);
                    }
                    else
                    {
                        if (pAxis->plPdTargetVelocity != EC_NULL) EC_SET_FRM_DWORD(pAxis->plPdTargetVelocity, -pMove->lConstantVel);
                    }
                }
                // Take back halt
                pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~(DRV_CRTL_HALT));

                // Goto waiting for acknowledge
                pMove->eProfileState = eProfileStatus_WaitForAck;

                break;

            case eProfileStatus_WaitForAck:
                if (pAxis->wStatusWord & DRV_STAT_OP_MODE_PP_ACK)
                {
                    pMove->eProfileState = eProfileStatus_WaitReached;
                }
                else
                {
                    // Stay in this state
                }
                break;

            case eProfileStatus_WaitReached:
                if (pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STOPPING)
                {
                    // Stop command was issued during movement
                    // Issue a quick stop
                    pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~DRV_CTRL_CMD_QUICK_STOP);
                    // Take back start movement commando
                    pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~DRV_CTRL_PP_NEW_SET_POINT);
                    // Switch to waiting for stop end
                    pMove->eProfileState = eProfileStatus_WaitQStop;
                }
                // Wait until movement ends
                if (pAxis->wStatusWord & DRV_STAT_VELOCITY_ZERO)
                {
                    // Take back start movement commando
                    pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~DRV_CTRL_PP_NEW_SET_POINT);
                    // Switch to idle state
                    pMove->eProfileState = eProfileStatus_Idle;
                }
                // Check for errors
                if (pAxis->wStatusWord & DRV_STAT_FOLLOW_ERR)
                {
                    // Take back start movement commando
                    pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~DRV_CTRL_PP_NEW_SET_POINT);

                    // Return to previous mode
                    pAxis->wProfileOperationMode = pAxis->wActOpMode;

                    // Switch to error state
                    pMove->eProfileState = eProfileStatus_Error;
                }
                break;

            case eProfileStatus_WaitQStop:
                // Wait until quick stop ends
                if (pAxis->wStatusWord & DRV_STAT_SWITCH_ON_DIS)
                {
                    // Switch to idle state
                    pMove->eProfileState = eProfileStatus_Idle;
                }
                else
                {
                    // Stay in this state
                }
                break;

            case eProfileStatus_Halt:
                // Command HALT
                pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord | DRV_CRTL_HALT);
                if (pAxis->plPdTargetVelocity != EC_NULL) EC_SET_FRM_DWORD(pAxis->plPdTargetVelocity, 0);
                // Switch to wait state
                pMove->eProfileState = eProfileStatus_WaitHaltDone;
                break;

            case eProfileStatus_WaitHaltDone:
                // Wait until movement ends
                if (pAxis->wStatusWord & DRV_STAT_VELOCITY_ZERO)
                {
                    // Switch to idle state
                    pMove->eProfileState = eProfileStatus_Idle;
                }
                else
                {
                    // Stay in this state
                }
                break;

            default:
                OsDbgAssert(EC_FALSE);
                break;
            }
        }
    }

    return;
}
#endif

#ifdef EC_MOTION_SUPPORT_MC_CAMMING
// band_matrix implementation
// -------------------------
band_matrix::band_matrix(MC_T_INT dim)
{
   m_upper_size = 0;
   m_lower_size = 0;
   resize(dim);
}

EC_T_DWORD band_matrix::resize(MC_T_INT dim)
{
    EC_T_DWORD      dwRes = MC_NO_ERROR;
    MC_T_INT        n_u = 1;
    MC_T_INT        n_l = 1;

    if(!(dim>0) || !(n_u>=0) || !(n_l>=0))
    {
       dwRes = MC_ERR_INVALID_PARAMETER;
       goto Exit;
    }

    m_upper_size = (n_u+1);
    m_lower_size = (n_l+1);
    m_dim = dim;

Exit:
    return dwRes;
}


// defines the new operator (), so that we can access the elements
// by A(i,j), index going from i=0,...,m_dim-1
MC_T_REAL & band_matrix::operator () (MC_T_INT i, MC_T_INT j)
{
    MC_T_INT k=j-i;       // what band is the entry

    assert( (i>=0) && (i<m_dim) && (j>=0) && (j<m_dim) );
    assert( (-num_lower()<=k) && (k<=num_upper()) );

    // k=0 -> diagonal, k<0 lower left part, k>0 upper right part
    if(k>=0) return m_upper[k][i];
    else     return m_lower[-k][i];
}

MC_T_REAL band_matrix::operator () (MC_T_INT i, MC_T_INT j) const
{
    MC_T_INT k=j-i;       // what band is the entry

    assert( (i>=0) && (i<m_dim) && (j>=0) && (j<m_dim) );
    assert( (-num_lower()<=k) && (k<=num_upper()) );

    // k=0 -> diagonal, k<0 lower left part, k>0 upper right part
    if(k>=0) return m_upper[k][i];
    else     return m_lower[-k][i];
}

// second diag (used in LU decomposition), saved in m_lower
MC_T_REAL band_matrix::saved_diag(MC_T_INT i) const
{
    assert( (i>=0) && (i<m_dim) );
    return m_lower[0][i];
}

MC_T_REAL & band_matrix::saved_diag(MC_T_INT i)
{
    assert( (i>=0) && (i<m_dim) );
    return m_lower[0][i];
}

// LR-Decomposition of a band matrix
void band_matrix::lu_decompose()
{
    MC_T_INT  i_max,j_max;
    MC_T_INT  j_min;
    MC_T_REAL x;

    // preconditioning
    // normalize column i so that a_ii=1
    for(MC_T_INT i=0; i < m_dim; i++)
    {
        assert(this->operator()(i,i)!=0.0);
        saved_diag(i)=1.0/this->operator()(i,i);
        j_min=EC_MAX(0, i - num_lower());
        j_max=EC_MIN(m_dim-1, i + num_upper());

        for(MC_T_INT j= j_min; j <= j_max; j++)
        {
            this->operator()(i,j) *= saved_diag(i);
        }
        this->operator()(i,i)=1.0;          // prevents rounding errors
    }

    // Gauss LR-Decomposition
    for(MC_T_INT k=0; k < m_dim; k++)
    {
        i_max=EC_MIN(m_dim-1,k+num_lower());  // num_lower not a mistake!
        for(MC_T_INT i=k+1; i<=i_max; i++)
        {
            assert(this->operator()(k,k)!=0.0);
            x=-this->operator()(i,k)/this->operator()(k,k);
            this->operator()(i,k)=-x;                         // assembly part of L
            j_max=EC_MIN(m_dim-1,k+num_upper());
            for(MC_T_INT j=k+1; j<=j_max; j++)
            {
                // assembly part of R
                this->operator()(i,j)=this->operator()(i,j)+x*this->operator()(k,j);
            }
        }
    }
}

// solves Ly=b
EC_T_VOID band_matrix::l_solve(MC_T_REAL* pB, MC_T_REAL*  pX)
{
    MC_T_INT               j_start;
    MC_T_REAL              sum;

    for(MC_T_INT i=0; i < m_dim; i++)
    {
        sum=0;
        j_start = EC_MAX(0,i - num_lower());
        for(MC_T_INT j = j_start; j<i; j++) sum += this->operator()(i,j)*pX[j];
        pX[i]=(pB[i]*saved_diag(i)) - sum;
    }
}

// solves Rx=y
EC_T_VOID band_matrix::r_solve(MC_T_REAL* pB, MC_T_REAL*  pX)
{
    MC_T_INT               j_stop;
    MC_T_REAL              sum;

    for(MC_T_INT i = m_dim-1; i>=0; i--)
    {
        sum = 0;
        j_stop = EC_MIN(m_dim-1, i + num_upper());
        for(MC_T_INT j=i+1; j<=j_stop; j++) sum += this->operator()(i,j)*pX[j];
        pX[i]=( pB[i] - sum ) / this->operator()(i,i);
    }
}

EC_T_VOID band_matrix::lu_solve(MC_T_REAL* pB, MC_T_REAL*  pX,
        bool is_lu_decomposed)
{
   MC_T_REAL     aY[MAX_NUM_OF_SPLINE_POINTS];
   OsMemset(aY, 0, sizeof(aY));

    if(is_lu_decomposed==false)
    {
        lu_decompose();
    }
    l_solve(pB, aY);
    r_solve(aY, pX);
}


// spline implementation
// -----------------------
EC_T_DWORD MC_T_SPLINE::set_boundary(MC_T_SPLINE::bd_type left, MC_T_REAL left_value,
                          MC_T_SPLINE::bd_type right, MC_T_REAL right_value,
                          MC_T_BOOL force_linear_extrapolation)
{
    EC_T_DWORD      dwRes = MC_NO_ERROR;

    // set_points() must not have happened yet
    if(m_nNumOfPoints != 0)
    {
       dwRes = MC_ERR_INVALID_PARAMETER;
       return dwRes;
    }

    m_left=left;
    m_right=right;
    m_left_value=left_value;
    m_right_value=right_value;
    m_force_linear_extrapolation=force_linear_extrapolation;

    return dwRes;
}


MC_T_WORD MC_T_SPLINE::set_points
   (MC_T_INT   nNumOfPoints,
    MC_T_REAL*  pX,
    MC_T_REAL*  pY,
    MC_T_BOOL cubic_spline)
{
    EC_T_WORD       wRes = MC_NO_ERROR;
    MC_T_REAL       h = 0.0;
    MC_T_INT        i=0;

    if(nNumOfPoints < 2)
    {
       wRes = MC_ERR_CAM_TABLE_ELEM_TOO_LESS;
       goto Exit;
    }

    if(nNumOfPoints > MAX_NUM_OF_SPLINE_POINTS)
    {
       wRes = MC_ERR_CAM_TABLE_ELEM_TOO_MANY;
       goto Exit;
    }

    m_nNumOfPoints = nNumOfPoints;
    OsMemcpy(m_x, pX, sizeof(MC_T_REAL) * nNumOfPoints);
    OsMemcpy(m_y, pY, sizeof(MC_T_REAL) * nNumOfPoints);

    // maybe sort x and y, rather than returning an error
    for(i=0; i < nNumOfPoints-1; i++)
    {
        assert(m_x[i] < m_x[i+1]);
    }

    if(cubic_spline==MC_TRUE) { // cubic spline interpolation
        // setting up the matrix and right hand side of the equation system
        // for the parameters b[]

        band_matrix   A(nNumOfPoints);
        MC_T_REAL     rhs[MAX_NUM_OF_SPLINE_POINTS];

        for(i = 1; i < nNumOfPoints-1; i++)
        {
           A(i,i-1) = 1.0/3.0*(m_x[i]-m_x[i-1]);
           A(i,i)   = 2.0/3.0*(m_x[i+1]-m_x[i-1]);
           A(i,i+1) = 1.0/3.0*(m_x[i+1]-m_x[i]);
            rhs[i]=(m_y[i+1]-m_y[i])/(m_x[i+1]-m_x[i]) - (m_y[i]-m_y[i-1])/(m_x[i]-m_x[i-1]);
        }
        // boundary conditions
        if(m_left == MC_T_SPLINE::second_deriv) {
            // 2*b[0] = f''
           A(0,0)=2.0;
           A(0,1)=0.0;
            rhs[0]=m_left_value;
        } else if(m_left == MC_T_SPLINE::first_deriv) {
            // c[0] = f', needs to be re-expressed in terms of b:
            // (2b[0]+b[1])(x[1]-x[0]) = 3 ((y[1]-y[0])/(x[1]-x[0]) - f')
           A(0,0)=2.0*(m_x[1]-m_x[0]);
           A(0,1)=1.0*(m_x[1]-m_x[0]);
            rhs[0]=3.0*((m_y[1]-m_y[0])/(m_x[1]-m_x[0])-m_left_value);
        } else {
            assert(false);
        }
        if(m_right == MC_T_SPLINE::second_deriv) {
            // 2*b[n-1] = f''
            A(nNumOfPoints-1,(EC_T_UINT)nNumOfPoints-1)=2.0;
            A(nNumOfPoints-1,(EC_T_UINT)nNumOfPoints-2)=0.0;
            rhs[nNumOfPoints-1]=m_right_value;
        } else if(m_right == MC_T_SPLINE::first_deriv) {
            // c[n-1] = f', needs to be re-expressed in terms of b:
            // (b[n-2]+2b[n-1])(x[n-1]-x[n-2])
            // = 3 (f' - (y[n-1]-y[n-2])/(x[n-1]-x[n-2]))
            A(nNumOfPoints-1, (EC_T_UINT)nNumOfPoints-1)=2.0*(m_x[nNumOfPoints-1]-m_x[nNumOfPoints-2]);
            A(nNumOfPoints-1, (EC_T_UINT)nNumOfPoints-2)=1.0*(m_x[nNumOfPoints-1]-m_x[nNumOfPoints-2]);
            rhs[nNumOfPoints-1]=3.0*(m_right_value-(m_y[nNumOfPoints-1]-m_y[nNumOfPoints-2])/(m_x[nNumOfPoints-1]-m_x[nNumOfPoints-2]));
        } else {
            assert(false);
        }

        // solve the equation system to obtain the parameters b[]
        A.lu_solve(rhs, m_b);

        // calculate parameters a[] and c[] based on b[]
        for (i = 0; i < nNumOfPoints - 1; i++)
        {
            m_a[i]=1.0/3.0*(m_b[i+1]-m_b[i])/(m_x[i+1]-m_x[i]);
            m_c[i]=(m_y[i+1]-m_y[i])/(m_x[i+1]-m_x[i])
                   - 1.0/3.0*(2.0*m_b[i]+m_b[i+1])*(m_x[i+1]-m_x[i]);
        }
    } else { // linear interpolation
        for (i = 0; i < nNumOfPoints - 1; i++) {
            m_a[i]=0.0;
            m_b[i]=0.0;
            m_c[i]=(m_y[i+1]-m_y[i])/(m_x[i+1]-m_x[i]);
        }
    }

    // for left extrapolation coefficients
    m_b0 = (m_force_linear_extrapolation==false) ? m_b[0] : 0.0;
    m_c0 = m_c[0];

    // for the right extrapolation coefficients
    // f_{n-1}(x) = b*(x-x_{n-1})^2 + c*(x-x_{n-1}) + y_{n-1}
    h=m_x[nNumOfPoints-1]-m_x[nNumOfPoints-2];
    // m_b[n-1] is determined by the boundary condition
    m_a[nNumOfPoints-1]=0.0;
    m_c[nNumOfPoints-1]=3.0*m_a[nNumOfPoints-2]*h*h+2.0*m_b[nNumOfPoints-2]*h+m_c[nNumOfPoints-2];   // = f'_{n-2}(x_{n-1})
    if(m_force_linear_extrapolation==MC_TRUE)
        m_b[nNumOfPoints-1]=0.0;


Exit:
    return wRes;
}

MC_T_REAL MC_T_SPLINE::get_result(MC_T_REAL x) const
{
    MC_T_INT nIdx;
    MC_T_REAL interpol;

    size_t n = m_nNumOfPoints;
    // find the closest point m_x[idx] < x, idx=0 even if x<m_x[0]
    //std::vector<MC_T_REAL>::const_iterator it;
    for (nIdx = 0; nIdx < (MC_T_INT)n; nIdx++)
    {
        if (m_x[nIdx] >= x)
        {
            break;
        }
    }
    nIdx = EC_MAX(nIdx - 1, 0);

    MC_T_REAL h = x - m_x[nIdx];
    if (x < m_x[0])
    {
        // extrapolation to the left
        interpol = (m_b0*h + m_c0)*h + m_y[0];
    }
    else if (x > m_x[n - 1])
    {
        // extrapolation to the right
        interpol = (m_b[n - 1] * h + m_c[n - 1])*h + m_y[n - 1];
    }
    else
    {
        // interpolation
        interpol = ((m_a[nIdx] * h + m_b[nIdx])*h + m_c[nIdx])*h + m_y[nIdx];
    }
    return interpol;
}
#endif //EC_MOTION_SUPPORT_MC_CAMMING



/*-END OF SOURCE FILE--------------------------------------------------------*/
