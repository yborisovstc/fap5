MyRoot : Elem {
    Launcher : DesLauncher {
        LogLevel = "Info"
        Ds1 : Des {
            Ds1_2 : Des {
                Ds1_3 : Des {
                    Ds1_4 : Des {
                        Ds1_5 : Des {
                            Ds1_6 : Des {
                                Ds1_7 : Des {
                                    Ds1_8 : Des {
                                        Ds1_9 : Des {
                                            St1 : State (
                                                _@ <  {
                                                    = "SI 0"
                                                    LogLevel = "Info"
                                                }
                                                Inp ~ Tradd : TrAdd2Var (
                                                    Inp ~ St1
                                                    Inp2 ~ : SI_1
                                                )
                                            )
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
