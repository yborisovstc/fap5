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
                    Cmp2_2 : Node {
                        Cmp2_2_0 : Node
                    }
                }
                # "Target_3"
                Target3 : Elem3 {
                    Cmp3_0 : Node
                    Cmp3_1 : Node
                    Cmp3_2 : Node
                    Cmp3_3 : Node {
                        Cmp3_3_0 : Node
                    }
                }
                # "Target_4"
                Target4 : Des {
                    Ext1 : ExtdStateOutp
                    St1 : State {
                        = "SI 1"
                    }
                    Ext1.Int ~ St1
                }
                # "Target_5"
                Target5 : Target4
            }
            # "Controller uses adapter for access to target"
            Adapter : DAdp {
                LogLevel = "Dbg"
                MName : SdoName
                CompsCount : SdoCompsCount
                CompNames : SdoCompsNames
                Parent : SdoParent
                Parents : SdoParents
                CompsUri : SdoCompsUri
                ExUri : SdoUri
                CompOwner : SdoCompOwner (
                    Comp ~ : Const {
                        = "URI Cmp3_3.Cmp3_3_0"
                    }
                )
                CompExists : SdoComp (
                    Name ~ : Const {
                        = "URI Cmp3_3.Cmp3_3_0"
                    }
                )
                AddComp : ASdcComp {
                    LogLevel = "Dbg"
                }
                CComp : SdoCompComp (
                    Comp ~ : Const {
                        = "URI Cmp3_3"
                    }
                    CompComp ~ : Const {
                        = "URI Cmp3_3_0"
                    }
                )
                ConnExists : SdoConn (
                    Vp ~ : Const {
                        = "URI St1"
                    }
                    Vq ~ : Const {
                        = "URI Ext1.Int"
                    }
                )
                PairsCount : SdoPairsCount (
                    Vp ~ : Const {
                        = "URI St1"
                    }
                )
                VPair : SdoPair (
                    Vp ~ : Const {
                        = "URI St1"
                    }
                )
                TcPair : SdoTcPair (
                    Targ ~ : Const {
                        = "URI Ext1"
                    }
                    TargComp ~ : Const {
                        = "URI Int"
                    }
                )
                MagPairs : SdoPairs
                TPairs : SdoTPairs (
                    Targ ~ : Const {
                        = "URI St1"
                    }
                )
                MagEdges : SdoEdges
                DesIsIdle : SdoDesIdle
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
                Inp ~ Adapter.MName
            )
            # "Uri"
            ExUri : State (
                _@ < LogLevel = "Dbg"
                _@ < = "URI"
                Inp ~ Adapter.ExUri
            )
            # "CompOwner"
            CompOwner : State (
                _@ < LogLevel = "Dbg"
                _@ < = "URI"
                Inp ~ Adapter.CompOwner
            )
            # "CompsUri"
            CompsUri : State (
                _@ < LogLevel = "Dbg"
                _@ < = "VDU"
                Inp ~ Adapter.CompsUri
            )
            # "CompComp"
            CompComp : State (
                _@ < LogLevel = "Dbg"
                _@ < = "URI"
                Inp ~ Adapter.CComp
            )
            # "Comp"
            CompExists : State (
                _@ < LogLevel = "Dbg"
                _@ < = "SB"
                Inp ~ Adapter.CompExists
            )
            # "Conn exists"
            ConnExists : State (
                _@ < LogLevel = "Dbg"
                _@ < = "SB"
                Inp ~ Adapter.ConnExists
            )
            # "Pairs count"
            PairsCount : State (
                _@ < LogLevel = "Dbg"
                _@ < = "SI"
                Inp ~ Adapter.PairsCount
            )
            # "Vertex pair"
            VPair : State (
                _@ < LogLevel = "Dbg"
                _@ < = "URI"
                Inp ~ Adapter.VPair
            )
            # "Tagtet comp pair"
            TcPair : State (
                _@ < LogLevel = "Dbg"
                _@ < = "URI"
                Inp ~ Adapter.TcPair
            )
            # "Mag pairs"
            MagPairs : State (
                _@ < LogLevel = "Dbg"
                _@ < = "VDU"
                Inp ~ Adapter.MagPairs
            )
            # "Targ pairs"
            TPairs : State (
                _@ < LogLevel = "Dbg"
                _@ < = "VDU"
                Inp ~ Adapter.TPairs
            )
            # "Mag edges"
            MagEdges : State (
                _@ < LogLevel = "Dbg"
                _@ < = "VPDU"
                Inp ~ Adapter.MagEdges
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
            # "Des is idle"
            DesIsIdle : State (
                _@ < LogLevel = "Dbg"
                _@ < = "SB"
                Inp ~ Adapter.DesIsIdle
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
