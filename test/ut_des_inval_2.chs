MyRoot : Elem {
    LogLevel = "Dbg"
    Launcher : DesLauncher {
        LogLevel = "Dbg"
        Ds1 : Des {
            Const_1 : SI_1
            St1 : State (
                _@ <  {
                    = "SI 0"
                    LogLevel = "Dbg"
                }
                Ext1 : ExtdStateOutp (
                    Int ~ St1
                )
                Ext2 : ExtdStateOutp
                Ext3 : ExtdStateOutp
                Ext2.Int ~ Ext1
                Ext3.Int ~ Ext2
                Inp ~ Add : TrAddVar (
                    Inp ~ Ext3
                    Inp ~ Const_1
                )
            )
        }
    }
}
