#include <abb_librws/v1_0/rws.h>

#include <boost/throw_exception.hpp>

#include <stdexcept>
#include <iostream>


namespace abb :: rws :: v1_0
{
  const unsigned short DEFAULT_PORT_NUMBER = 80;
  const std::string DEFAULT_PASSWORD       = "robotics";
  const std::string DEFAULT_USERNAME       = "Default User";

  const std::string Identifiers::ACTIVE                         = "active";
  const std::string Identifiers::ARM                            = "arm";
  const std::string Identifiers::CFG_DT_INSTANCE_LI             = "cfg-dt-instance-li";
  const std::string Identifiers::CFG_IA_T_LI                    = "cfg-ia-t-li";
  const std::string Identifiers::CTRL_TYPE                      = "ctrl-type";
  const std::string Identifiers::CTRLEXECSTATE                  = "ctrlexecstate";
  const std::string Identifiers::CTRLSTATE                      = "ctrlstate";
  const std::string Identifiers::DATTYP                         = "dattyp";
  const std::string Identifiers::EXCSTATE                       = "excstate";
  const std::string Identifiers::IOS_SIGNAL                     = "ios-signal";
  const std::string Identifiers::HOME_DIRECTORY                 = "$home";
  const std::string Identifiers::LVALUE                         = "lvalue";
  const std::string Identifiers::MECHANICAL_UNIT                = "mechanical_unit";
  const std::string Identifiers::MECHANICAL_UNIT_GROUP          = "mechanical_unit_group";
  const std::string Identifiers::MOC                            = "moc";
  const std::string Identifiers::MODULE                         = "module";
  const std::string Identifiers::MODULEPATH                     = "modulepath";
  const std::string Identifiers::MOTIONTASK                     = "motiontask";
  const std::string Identifiers::NAME                           = "name";
  const std::string Identifiers::OPMODE                         = "opmode";
  const std::string Identifiers::PRESENT_OPTIONS                = "present_options";
  const std::string Identifiers::RAP_MODULE_INFO_LI             = "rap-module-info-li";
  const std::string Identifiers::RAP_TASK_LI                    = "rap-task-li";
  const std::string Identifiers::RESTART_MODE                   = "restart-mode";
  const std::string Identifiers::ROBOT                          = "robot";
  const std::string Identifiers::RW_VERSION_NAME                = "rwversionname";
  const std::string Identifiers::SINGLE                         = "single";
  const std::string Identifiers::STATE                          = "state";
  const std::string Identifiers::SYS                            = "sys";
  const std::string Identifiers::SYS_OPTION_LI                  = "sys-option-li";
  const std::string Identifiers::SYS_SYSTEM_LI                  = "sys-system-li";
  const std::string Identifiers::TITLE                          = "title";
  const std::string Identifiers::TYPE                           = "type";
  const std::string Identifiers::VALUE                          = "value";
  const std::string Identifiers::CLASS                          = "class";
  const std::string Identifiers::OPTION                         = "option";
  const std::string Identifiers::PCP_INFO                       = "pcp-info";
  const std::string Identifiers::BEGIN_POSITION                 = "beginposition";
  const std::string Identifiers::END_POSITION                   = "endposition";
  const std::string Identifiers::MODULE_NAME                    = "modulemame";
  const std::string Identifiers::ROUTINE_NAME                   = "routinename";
  const std::string Queries::ACTION_LOAD_MODULE                 = "action=loadmod";
  const std::string Queries::ACTION_RELEASE                     = "action=release";
  const std::string Queries::ACTION_REQUEST                     = "action=request";
  const std::string Queries::ACTION_RESETPP                     = "action=resetpp";
  const std::string Queries::ACTION_SET                         = "action=set";
  const std::string Queries::ACTION_SETCTRLSTATE                = "action=setctrlstate";
  const std::string Queries::ACTION_SET_LOCALE                  = "action=set-locale";
  const std::string Queries::ACTION_START                       = "action=start";
  const std::string Queries::ACTION_STOP                        = "action=stop";
  const std::string Queries::ACTION_UNLOAD_MODULE               = "action=unloadmod";
  const std::string Queries::TASK                               = "task=";
  const std::string Services::CTRL                              = "/ctrl";
  const std::string Services::FILESERVICE                       = "/fileservice";
  const std::string Services::RW                                = "/rw";
  const std::string Services::SUBSCRIPTION                      = "/subscription";
  const std::string Services::USERS                             = "/users";
  const std::string Resources::INSTANCES                        = "/instances";
  const std::string Resources::JOINTTARGET                      = "/jointtarget";
  const std::string Resources::LOGOUT                           = "/logout";
  const std::string Resources::ROBTARGET                        = "/robtarget";
  const std::string Resources::RW_CFG                           = Services::RW + "/cfg";
  const std::string Resources::RW_PANEL                         = Services::RW + "/panel";
  const std::string Resources::RW_IOSYSTEM_SIGNALS              = Services::RW + "/iosystem/signals";
  const std::string Resources::RW_MASTERSHIP                    = Services::RW + "/mastership";
  const std::string Resources::RW_MOTIONSYSTEM_MECHUNITS        = Services::RW + "/motionsystem/mechunits";
  const std::string Resources::RW_PANEL_CTRLSTATE               = Resources::RW_PANEL + "/ctrlstate";
  const std::string Resources::RW_PANEL_OPMODE                  = Resources::RW_PANEL + "/opmode";
  const std::string Resources::RW_RAPID_EXECUTION               = Services::RW + "/rapid/execution";
  const std::string Resources::RW_RAPID_MODULES                 = Services::RW + "/rapid/modules";
  const std::string Resources::RW_RAPID_SYMBOL_DATA_RAPID       = Services::RW + "/rapid/symbol/data/RAPID";
  const std::string Resources::RW_RAPID_SYMBOL_PROPERTIES_RAPID = Services::RW + "/rapid/symbol/properties/RAPID";
  const std::string Resources::RW_RAPID_TASKS                   = Services::RW + "/rapid/tasks";
  const std::string Resources::RW_SYSTEM                        = Services::RW + "/system";

  const XMLAttribute XMLAttributes::CLASS_ACTIVE(Identifiers::CLASS            , Identifiers::ACTIVE);
  const XMLAttribute XMLAttributes::CLASS_CFG_DT_INSTANCE_LI(Identifiers::CLASS, Identifiers::CFG_DT_INSTANCE_LI);
  const XMLAttribute XMLAttributes::CLASS_CFG_IA_T_LI(Identifiers::CLASS       , Identifiers::CFG_IA_T_LI);
  const XMLAttribute XMLAttributes::CLASS_CTRL_TYPE(Identifiers::CLASS         , Identifiers::CTRL_TYPE);
  const XMLAttribute XMLAttributes::CLASS_CTRLEXECSTATE(Identifiers::CLASS     , Identifiers::CTRLEXECSTATE);
  const XMLAttribute XMLAttributes::CLASS_CTRLSTATE(Identifiers::CLASS         , Identifiers::CTRLSTATE);
  const XMLAttribute XMLAttributes::CLASS_DATTYP(Identifiers::CLASS            , Identifiers::DATTYP);
  const XMLAttribute XMLAttributes::CLASS_EXCSTATE(Identifiers::CLASS          , Identifiers::EXCSTATE);
  const XMLAttribute XMLAttributes::CLASS_IOS_SIGNAL(Identifiers::CLASS        , Identifiers::IOS_SIGNAL);
  const XMLAttribute XMLAttributes::CLASS_LVALUE(Identifiers::CLASS            , Identifiers::LVALUE);
  const XMLAttribute XMLAttributes::CLASS_MOTIONTASK(Identifiers::CLASS        , Identifiers::MOTIONTASK);
  const XMLAttribute XMLAttributes::CLASS_NAME(Identifiers::CLASS              , Identifiers::NAME);
  const XMLAttribute XMLAttributes::CLASS_OPMODE(Identifiers::CLASS            , Identifiers::OPMODE);
  const XMLAttribute XMLAttributes::CLASS_RAP_MODULE_INFO_LI(Identifiers::CLASS, Identifiers::RAP_MODULE_INFO_LI);
  const XMLAttribute XMLAttributes::CLASS_RAP_TASK_LI(Identifiers::CLASS       , Identifiers::RAP_TASK_LI);
  const XMLAttribute XMLAttributes::CLASS_RW_VERSION_NAME(Identifiers::CLASS   , Identifiers::RW_VERSION_NAME);
  const XMLAttribute XMLAttributes::CLASS_STATE(Identifiers::CLASS             , Identifiers::STATE);
  const XMLAttribute XMLAttributes::CLASS_SYS_OPTION_LI(Identifiers::CLASS     , Identifiers::SYS_OPTION_LI);
  const XMLAttribute XMLAttributes::CLASS_SYS_SYSTEM_LI(Identifiers::CLASS     , Identifiers::SYS_SYSTEM_LI);
  const XMLAttribute XMLAttributes::CLASS_TYPE(Identifiers::CLASS              , Identifiers::TYPE);
  const XMLAttribute XMLAttributes::CLASS_VALUE(Identifiers::CLASS             , Identifiers::VALUE);
  const XMLAttribute XMLAttributes::CLASS_OPTION(Identifiers::CLASS            , Identifiers::OPTION);
  const XMLAttribute XMLAttributes::CLASS_PCP_INFO(Identifiers::CLASS          , Identifiers::PCP_INFO);
  const XMLAttribute XMLAttributes::CLASS_BEGIN_POSITION(Identifiers::CLASS    , Identifiers::BEGIN_POSITION);
  const XMLAttribute XMLAttributes::CLASS_END_POSITION(Identifiers::CLASS      , Identifiers::END_POSITION);
  const XMLAttribute XMLAttributes::CLASS_MODULE_NAME(Identifiers::CLASS       , Identifiers::MODULE_NAME);
  const XMLAttribute XMLAttributes::CLASS_ROUTINE_NAME(Identifiers::CLASS      , Identifiers::ROUTINE_NAME);


  std::ostream& operator<<(std::ostream& os, MastershipDomain domain)
  {
    switch (domain)
    {
      case MastershipDomain::cfg:
        os << "cfg";
        break;
      case MastershipDomain::motion:
        os << "motion";
        break;
      case MastershipDomain::rapid:
        os << "rapid";
        break;
      default:
        BOOST_THROW_EXCEPTION(std::logic_error {"Invalid v1_0::MastershipDomain value"});
    }

    return os;
  }
}