Root : Elem {
    # "Test of SDO #2, edges"
    Launcher : DesLauncher {
        # "Components to be checked"
        V1 : Vert
        V2 : Vert
        V3 : Vert
        V4 : Vert {
            V4_1 : Vert
        }
        V5 : Vert {
            V5_1 : Vert
        }
        V6 : Vert
        V1 ~ V2
        V1 ~ V4
        V5 ~ V4
        V5 ~ V3
        V4.V4_1 ~ V6
        # "Checking existence of component"
        Dbg_DcoEdges : State (
            _@ <  {
                LogLevel = "Dbg"
                = "VPDU _INV"
            }
            Inp ~ DcoComp : SdoEdges (
                _@ < LogLevel = "Dbg"
            )
        )
    }
}
