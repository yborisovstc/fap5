testroot : Elem {
    test : DesLauncher {
        LogLevel = "Dbg"
        # "MNode adapter test: observation of compoments number"
        # "Managed agent is not adapter owned - using input to access managed agent top owner"
        Controller : Des {
            Elem1 : Elem
            Elem2 : Elem1
            Elem3 : Elem2
            Targets : Syst {
                CpExplorable : CpSystExplorable
                # "Target_1"
                Target : Node {
                    Cmp_0 : Node
                    Cmp_1 : Node
                }
                # "Target_2"
                Target2 : Node {
                    Cmp2_0 : Node
                    Cmp2_1 : Node
                    Cmp2_2 : Node
                }
                # "Target_3"
                Target3 : Elem3 {
                    Cmp3_0 : Node
                    Cmp3_1 : Node
                    Cmp3_2 : Node
                    Cmp3_3 : Node
                }
                # "Target_4"
                Target4 : Des {
                    St1 : State {
                        = "SI 1"
                    }
                }
                # "Target_5"
                Target5 : Target4
            }
            # "Controller uses adapter for access to target"
            Adapter : DAdp {
                LogLevel = "Dbg"
                AgentUri = "Target"
                Name : SdoName
                CompsCount : SdoCompsCount
                CompNames : SdoCompsNames
                Parent : SdoParent
                Parents : SdoParents
                AddComp : ASdcComp {
                    LogLevel = "Dbg"
                }
            }
            Adapter.CpExploring ~ Targets.CpExplorable
            # "Components_count"
            CompCount : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SI 0"
                }
                Inp ~ Adapter.CompsCount
            )
            # "Components"
            CompNames : State (
                _@ <  {
                    LogLevel = "Dbg"
                    _@ < = "VS"
                }
                Inp ~ Adapter.CompNames
            )
            # "Name"
            Name_Dbg : State (
                _@ < LogLevel = "Dbg"
                _@ < = "SS"
                Inp ~ Adapter.Name
            )
            # "Parent"
            Parent_Dbg : State (
                _@ < LogLevel = "Dbg"
                _@ < = "SS"
                Inp ~ Adapter.Parent
            )
            # "Parents"
            Parents_Dbg : State (
                _@ < LogLevel = "Dbg"
                _@ < = "VDU"
                Inp ~ Adapter.Parents
            )
            # "Tics_Counter"
            Counter : State {
                LogLevel = "Dbg"
                = "SI 0"
            }
            Incr : TrAddVar
            Counter.Inp ~ Incr
            Const_1 : State {
                = "SI 1"
            }
            Incr.Inp ~ Const_1
            Incr.Inp ~ Counter
            # "Managed agent 1 URI"
            MagUri : State {
                = "SS Target5"
            }
            # "Managed agent 2 URI"
            MagUri2 : State {
                = "SS Target3"
            }
            # "Target switcher"
            Const_3 : State {
                = "SI 3"
            }
            Sw : TrSwitchBool (
                _@ < LogLevel = "Dbg"
                Sel ~ Cmp_Ge : TrCmpVar (
                    Inp ~ Counter
                    Inp2 ~ Const_3
                )
                Inp1 ~ MagUri
                Inp2 ~ MagUri2
            )
            Adapter.InpMagUri ~ : TrToUriVar (
                Inp ~ Sw
            )
            # "Mutating: adding component"
            Adapter.AddComp.Name ~ : State {
                = "SS New_node"
            }
            Adapter.AddComp.Parent ~ : State {
                = "SS Node"
            }
            Adapter.AddComp.Enable ~ Cmp2_Eq : TrCmpVar (
                Inp ~ Counter
                Inp2 ~ : State {
                    = "SI 6"
                }
            )
            AddCompOutp_Dbg : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB"
                }
                Inp ~ Adapter.AddComp.Outp
            )
        }
    }
}
