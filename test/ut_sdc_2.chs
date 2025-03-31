Root : Elem {
    # "Test of SDC, list insertion/extraction"
    + DesUtils
    Launcher : DesLauncher {
        # "Node"
        LNode : Syst {
            Prev : ExtdStateOutp
            Next : ExtdStateInp
        }
        List : Des {
            Start : LNode
            End : LNode
            Start.Prev ~ End.Next
            # "Creating component"
            Dc_Comp : ASdcComp (
                _@ < LogLevel = "Dbg"
                Enable ~ : SB_True
                Name ~ NodeName : Const {
                    = "SS Node_1"
                }
                Parent ~ : Const {
                    = "SS LNode"
                }
            )
            Dc_Insert : ASdcInsert2 (
                _@ < LogLevel = "Dbg"
                Name ~ NodeName
                Prev ~ : Const {
                    = "SS Prev"
                }
                Next ~ : Const {
                    = "SS Next"
                }
                Pname ~ : Const {
                    = "SS End"
                }
            )
            Dc_Insert_Out_Dbg : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB false"
                }
                Inp ~ Dc_Insert.Outp
            )
            Dc_Extract : ASdcExtract (
                _@ < LogLevel = "Dbg"
                Enable ~ Dc_Insert.Outp
                Name ~ NodeName
                Prev ~ : Const {
                    = "SS Prev"
                }
                Next ~ : Const {
                    = "SS Next"
                }
            )
            Dc_Extract_Out_Dbg : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB false"
                }
                Inp ~ Dc_Extract.Outp
            )
            # "Enable insertion first on creation than on extraction"
            Dc_Insert.Enable ~ : TrSwitchBool (
                _@ < LogLevel = "Dbg"
                Inp1 ~ Dc_Comp.Outp
                Inp2 ~ Dc_Extract.Outp
                WasInserted : DesUtils.SetTg (
                    InpSet ~ Dc_Insert.Outp
                )
                Sel ~ WasInserted.Outp
            )
        }
    }
}
