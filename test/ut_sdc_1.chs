Root : Elem {
    # "Test of simplem System DES controller (DSC) model"
    Launcher : DesLauncher {
        # "Creating component"
        Dc_Comp_Err : ASdcComp (
            _@ < LogLevel = "Dbg"
            Enable ~ : Const {
                = "SB true"
            }
            Name ~ : Const {
                = "SS _INV"
            }
            Parent ~ : Const {
                = "SS Node"
            }
        )
        Dc_Comp_Err_Dbg : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SB false"
            }
            Inp ~ Dc_Comp_Err.Outp
        )
        # "Creating component Comp"
        Dc_Comp : ASdcComp (
            _@ < LogLevel = "Dbg"
            Enable ~ : Const {
                = "SB true"
            }
            Name ~ Comp1Name : Const {
                = "SS Comp"
            }
            Parent ~ : Const {
                = "SS Node"
            }
        )
        Dc_Comp_Dbg : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SB false"
            }
            Inp ~ Dc_Comp.Outp
        )
        Dc_Rm : ASdcRm (
            _@ < LogLevel = "Dbg"
            Enable ~ Dc_Comp.Outp
            Name ~ Comp1Name
        )
        # "Creating component Comp2"
        Dc_Comp2 : ASdcComp (
            _@ < LogLevel = "Dbg"
            Enable ~ : State {
                = "SB true"
            }
            Name ~ : State {
                = "SS Comp2"
            }
            Parent ~ : State {
                = "SS Node"
            }
        )
        Dc_Comp2_Dbg : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SB false"
            }
            Inp ~ Dc_Comp2.Outp
        )
        # "Connecting vert"
        Dc_V1 : ASdcComp (
            _@ < LogLevel = "Dbg"
            Enable ~ : State {
                = "SB true"
            }
            Name ~ V1Name : State {
                = "SS V1"
            }
            Parent ~ : State {
                = "SS Vert"
            }
        )
        Dc_V2 : ASdcComp (
            _@ < LogLevel = "Dbg"
            Enable ~ Dc_V1.Outp
            Name ~ V2Name : State {
                = "SS V2"
            }
            Parent ~ : State {
                = "SS Vert"
            }
        )
        Dc_Conn : ASdcConn (
            _@ < LogLevel = "Dbg"
            Enable ~ Dc_V2.Outp
            V1 ~ V1Name
            V2 ~ V2Name
        )
        Dc_Conn_Dbg : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SB false"
            }
            Inp ~ Dc_Conn.Outp
        )
    }
}
