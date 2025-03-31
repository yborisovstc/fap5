Root : Elem {
    # "Test of SDC, list insertion/extraction, number of comps"
    # "Ref DS_ISS_013 for the use-case"
    Launcher : DesLauncher {
        # "List Node"
        LNode : Syst {
            Prev : ExtdStateOutp
            Next : ExtdStateInp
        }
        List : Des {
            KS_Prev : State {
                = "SS Prev"
            }
            KS_Next : State {
                = "SS Next"
            }
            KS_End : State {
                = "SS End"
            }
            KS_LNode : State {
                = "SS LNode"
            }
            Start : LNode
            End : LNode
            Start.Prev ~ End.Next
            # "Creating components"
            Dc_Comp : ASdcComp (
                _@ < LogLevel = "Dbg"
                Enable ~ : State {
                    = "SB true"
                }
                Name ~ Node_1_name : State {
                    = "SS Node_1"
                }
                Parent ~ KS_LNode
            )
            Dc_Comp2 : ASdcComp (
                _@ < LogLevel = "Dbg"
                Enable ~ Dc_Comp.Outp
                Name ~ Node_2_name : State {
                    = "SS Node_2"
                }
                Parent ~ KS_LNode
            )
            # "With DS_ISS_013 this sdc will -insert- Node_1 twice, the second time after Node_2 inserted"
            Dc_Insert : ASdcInsert2 (
                _@ < LogLevel = "Dbg"
                Enable ~ Dc_Comp.Outp
                Name ~ Node_1_name
                Prev ~ KS_Prev
                Next ~ KS_Next
                Pname ~ KS_End
            )
            Dc_Insert_Out_Dbg : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB false"
                }
                Inp ~ Dc_Insert.Outp
            )
            Dc_Insert2 : ASdcInsert2 (
                _@ < LogLevel = "Dbg"
                Enable ~ Dc_Comp2.Outp
                Name ~ Node_2_name
                Prev ~ KS_Prev
                Next ~ KS_Next
                Pname ~ KS_End
            )
            Is_conn_ok : SdoConn (
                # "Verificator if connection in the list is correct"
                _@ < LogLevel = "Dbg"
                Vp ~ : Const {
                    = "URI Node_2.Prev"
                }
                Vq ~ : Const {
                    = "URI End.Next"
                }
            )
            Is_conn_ok_Dbg : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB"
                }
                Inp ~ Is_conn_ok
            )
            Is_conn_ok_Dbg2 : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB"
                }
                Inp ~ : TrAndVar (
                    Inp ~ Is_conn_ok
                    Inp ~ Dc_Comp2.Outp
                )
            )

        }
    }
}
