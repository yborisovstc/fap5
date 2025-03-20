MyRoot : Elem {
    Launcher : DesLauncher {
        Ds1 : Des {
            Sock1 : Socket {
                Pin : CpStateOutpPin
            }
            Sock2 : Socket {
                Pin : CpStateInpPin
            }
            St1 : State {
                = "SI 0"
                LogLevel = "Dbg"
            }
            Const_1 : State {
                = "SI 1"
            }
            Const_2 : State {
                = "SI 1"
            }
            Add : TrAddVar
            St1.Inp ~ Add
            Add.Inp ~ St1
            Add.Inp ~ Const_2
            Add.InpN ~ Sock1.Pin
            Sock2.Pin ~ Const_1
            Sock1 ~ Sock2
        }
    }
}
