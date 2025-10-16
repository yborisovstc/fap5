MyRoot : Elem {
    Launcher : DesLauncher {
        Ds1 : Des {
            _ <  {
                # "<<< Hidden"
            }
            St1 : State (
                _@ <  {
                    = "SB true"
                    LogLevel = "Dbg"
                }
                Inp ~ Neg : TrNegVar (
                    Inp ~ St1
                )
            )
            # "Data: Scalar: string"
            StSS : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SS"
                }
                Inp ~ : State {
                    = "SS 'Hello World!'"
                }
            )
            # "Data: Pair of strings"
            StPS : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "PS"
                }
                Inp ~ : State {
                    = "PS ( First , Second )"
                }
            )
            # "Data: Pair of scalar data: strings"
            StPSS : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "PSS"
                }
                Inp ~ : State {
                    = "PSS ( SS 'First elem' , SS 'Second elem' )"
                }
            )
            # "Data: Pair of data URI"
            StPU : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "PDU "
                }
                Inp ~ : State {
                    = "PDU ( URI first1.first2 , URI second1.second2 )"
                }
            )
            # "Data: Vector of strings"
            StVS : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "VS"
                }
                Inp ~ : State {
                    = "VS ( elem1 , elem2 , elem3 )"
                }
            )
            # "Data: Vector of URI"
            StVDU : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "VDU "
                }
                Inp ~ : State {
                    = "VDU ( URI e1_f1.e1_f2 , URI e2_f1.e2_f2 )"
                }
            )
            # "Data: Vector of Pair of URI"
            StVPU : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "VPDU "
                }
                Inp ~ : State {
                    = "VPDU ( PDU ( URI e1f1.e1f2 , URI e1s1.e1s2 ) , PDU ( URI e2f1.e2f2 , URI e2s1.e2s2 ) )"
                }
            )
            # "Data: Vector of Pair of string"
            StVPS : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "VPS "
                }
                Inp ~ : State {
                    = "VPS ( PS (e1_first , e1_second ) ,  PS (e2_first , e2_second ) )"
                }
            )
            # "URI"
            # "Data: URI"
            StUri : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "URI"
                }
                Inp ~ : State {
                    = "URI State1.State2"
                }
            )
            # "Transitions"
            St2 : State {
                = "VS ( Item_1 , Item_2 , Item_3 )"
            }
            # "Size of vector"
            SSizeOfVect : State (
                _@ <  {
                    = "SI -1"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrSizeVar (
                    Inp ~ St2
                )
            )
            # "Switch"
            SSwitch : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrSwitchBool (
                    Inp1 ~ : State {
                        = "SI 5"
                    }
                    Inp2 ~ : State {
                        = "SI 2"
                    }
                    Sel ~ : State {
                        = "SB true"
                    }
                )
            )
            # "Switch2"
            SSwitch2 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrSwitchBool2 (
                    Inp1 ~ : State {
                        = "SI 5"
                    }
                    Inp2 ~ : State {
                        = "SI 2"
                    }
                    Sel ~ : State {
                        = "SB true"
                    }
                )
            )
            # "Switch with unselected channel invalid"
            SSwitchUsInv : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrSwitchBool (
                    Inp1 ~ : State {
                        = "SI"
                    }
                    Inp2 ~ : State {
                        = "SI 2"
                    }
                    Sel ~ : State {
                        = "SB true"
                    }
                )
            )
            # "Add int"
            SAddInt : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrAddVar (
                    Inp ~ : State {
                        = "SI 5"
                    }
                    Inp ~ : State {
                        = "SI 2"
                    }
                )
            )
            # "Add int multidata, ds_dri_mcins"
            SAddIntMd : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrAddVar (
                    Inp ~ : ExtdStateOutp (
                        Int ~ : State {
                            = "SI 7"
                        }
                        Int ~ : State {
                            = "SI 4"
                        }
                    )
                    InpN ~ : ExtdStateOutp (
                        Int ~ : State {
                            = "SI 2"
                        }
                        Int ~ : State {
                            = "SI 3"
                        }
                    )
                )
            )
            # "Mpl int"
            SMplInt : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrMplVar (
                    Inp ~ : State {
                        = "SI 5"
                    }
                    Inp ~ : State {
                        = "SI 2"
                    }
                )
            )
            # "Div int"
            SDivInt : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrDivVar (
                    Inp ~ : State {
                        = "SI 10"
                    }
                    Inp2 ~ : State {
                        = "SI 2"
                    }
                )
            )
            # "Min int"
            SMinInt : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrMinVar (
                    Inp ~ : State {
                        = "SI 10"
                    }
                    Inp ~ : State {
                        = "SI 5"
                    }
                    Inp ~ : State {
                        = "SI 1"
                    }
                )
            )
            # "Max int"
            SMaxInt : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrMaxVar (
                    Inp ~ : State {
                        = "SI 10"
                    }
                    Inp ~ : State {
                        = "SI 5"
                    }
                    Inp ~ : State {
                        = "SI 1"
                    }
                )
            )
            # "And"
            SAnd : State (
                _@ <  {
                    = "SB"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrAndVar (
                    Inp ~ : State {
                        = "SB true"
                    }
                    Inp ~ : State {
                        = "SB true"
                    }
                    Inp ~ : State {
                        = "SB true"
                    }
                )
            )
            # "Or"
            SOr : State (
                _@ <  {
                    = "SB"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrOrVar (
                    Inp ~ : State {
                        = "SB false"
                    }
                    Inp ~ : State {
                        = "SB true"
                    }
                    Inp ~ : State {
                        = "SB false"
                    }
                )
            )
            # "Neg"
            SNeg : State (
                _@ <  {
                    = "SB"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrNegVar (
                    Inp ~ : State {
                        = "SB false"
                    }
                )
            )
            # "Uri"
            SUri : State (
                _@ <  {
                    = "URI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrToUriVar (
                    Inp ~ : State {
                        = "SS elem1.elem2.elem3"
                    }
                )
            )
            # "Item of vector, wrapped by Sdata"
            SVectItm : State (
                _@ <  {
                    = "SS"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrAtVar (
                    Inp ~ St2
                    Index ~ : State {
                        = "SI 2"
                    }
                )
            )
            # "Item of vector, generic: not wrapped by Sdata"
            SAtgVdu : State (
                _@ <  {
                    = "URI "
                    LogLevel = "Dbg"
                }
                Inp ~ : TrAtgVar (
                    Inp ~ : State {
                        = "VDU ( URI a1.a2 , URI b1.b2 )"
                    }
                    Index ~ : State {
                        = "SI 1"
                    }
                )
            )
            # "Item of Pair, generic: not wrapped by Sdata"
            SAtgPu : State {
                = "URI"
                LogLevel = "Dbg"
            }
            SAtgPu.Inp ~ : TrAtgVar (
                Inp ~ : State {
                    = "PDU ( URI a1.a2 , URI b1.b2 )"
                }
                Index ~ : State {
                    = "SI 1"
                }
            )
            # "Find in pairs vector by P pair element"
            SFindByPVpdu : State (
                _@ <  {
                    = "URI "
                    LogLevel = "Dbg"
                }
                Inp ~ : TrFindByP (
                    Inp ~ : State {
                        = "VPDU ( PDU ( URI e1f1.e1f2 , URI e1s1.e1s2 ) , PDU ( URI e2f1.e2f2 , URI e2s1.e2s2 )  , PDU ( URI e3f1.e3f2 , URI e3s1.e3s2 ) )"
                    }
                    Sample ~ : State {
                        = "URI e2f1.e2f2"
                    }
                )
            )
            # "Tuple composer"
            TupleRes : State (
                _@ <  {
                    = "TPL,SS:name,SI:value _INV 0"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrTuple (
                    Inp ~ : State {
                        = "TPL,SS:name,SI:value none 0"
                    }
                    _@ <  {
                        name : CpStateInp
                        value : CpStateInp
                    }
                    name ~ : State {
                        = "SS Test_name"
                    }
                    value ~ : State {
                        = "SI 24"
                    }
                )
            )
            # "Tuple composer, inp via conn"
            T2Data : State {
                = "SI 5"
            }
            T2extd : ExtdStateInp
            T2extd ~ T2Data
            TupleRes2 : State (
                _@ <  {
                    = "TPL,SI:data,SI:value  "
                    LogLevel = "Dbg"
                }
                Inp ~ : TrTuple (
                    Inp ~ : State {
                        = "TPL,SI:data,SI:value  "
                    }
                    _@ <  {
                        data : CpStateInp
                        value : CpStateInp
                    }
                    data ~ T2extd.Int
                    value ~ : TrAddVar (
                        Inp ~ : State {
                            = "SI 2"
                        }
                        Inp ~ : State {
                            = "SI 5"
                        }
                    )
                )
            )
            # "Tuple composer, missing inp"
            TupleRes3 : State (
                _@ <  {
                    = "TPL,SI:data,SI:value -1 -2"
                    LogLevel = "Dbg"
                }
                Inp ~ TupleCps3 : TrTuple (
                    Inp ~ : State {
                        = "TPL,SI:data,SI:value -1 -2"
                    }
                    _@ <  {
                        data : CpStateInp
                        value : CpStateInp
                    }
                    value ~ : State {
                        = "SI 2"
                    }
                )
            )
            # "Tuple component selector"
            TupleSelData_Dbg : State (
                _@ <  {
                    = "SI "
                    LogLevel = "Dbg"
                }
                Inp ~ TplSelData : TrTupleSel (
                    Inp ~ TupleRes2
                    Comp ~ : State {
                        = "SS data"
                    }
                )
            )
            # "Comparition"
            SCmpLt : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB"
                }
                Inp ~ CmpLt_Lt : TrCmpVar (
                    Inp ~ : State {
                        = "SI 5"
                    }
                    Inp2 ~ : State {
                        = "SI 7"
                    }
                )
            )
            SCmpNeqInv : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB"
                }
                Inp ~ Cmp_Neq_1 : TrCmpVar (
                    Inp ~ : State {
                        = "SI 5"
                    }
                    Inp2 ~ : State {
                        = "SI"
                    }
                )
            )
            # "Tuple component selector iface guessing"
            TupleSelIfaceG : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB"
                }
                Inp ~ ColPos_Lt : TrCmpVar (
                    Inp ~ : TrTupleSel (
                        Inp ~ TupleRes2
                        Comp ~ : State {
                            = "SS data"
                        }
                    )
                    Inp2 ~ : TrTupleSel (
                        Inp ~ TupleRes2
                        Comp ~ : State {
                            = "SS value"
                        }
                    )
                )
            )
            # "Intput counter"
            InpCntRes : State (
                _@ <  {
                    = "SI "
                    LogLevel = "Dbg"
                }
                Inp ~ : TrInpCnt (
                    Inp ~ : State {
                        = "SS Inp1"
                    }
                    Inp ~ : State {
                        = "SS Inp2"
                    }
                    Inp ~ : State {
                        = "SS Inp3"
                    }
                )
            )
            # "Intput selector"
            InpSelRes : State (
                _@ <  {
                    = "SS "
                    LogLevel = "Dbg"
                }
                Inp ~ : TrInpSel (
                    Inp ~ : State {
                        = "SS Inp1"
                    }
                    Inp ~ : State {
                        = "SS Inp2"
                    }
                    Inp ~ : State {
                        = "SS Inp3"
                    }
                    Idx ~ : State {
                        = "SI 2"
                    }
                )
            )
            # "Chromo data"
            SChrD : State (
                _@ <  {
                    = "CHR2"
                    LogLevel = "Dbg"
                }
                Inp ~ ChrD : State {
                    = "CHR2 '{ Text = \\\"Button 3\\\";  BgColor < { R = \\\"0.0\\\"; G = \\\"0.0\\\"; B = \\\"1.0\\\"; } }'"
                }
            )
            # "Chromo composer"
            ChromoRes : State (
                _@ <  {
                    = "CHR2"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrChr (
                    Mut ~ : TrMutNode (
                        Parent ~ : State {
                            = "SS Node"
                        }
                        Name ~ : State {
                            = "SS Test_Node"
                        }
                    )
                    Mut ~ : TrMutNode (
                        Parent ~ : State {
                            = "SS Test_Node"
                        }
                        Name ~ : State {
                            = "SS Test_Node2"
                        }
                    )
                )
            )
            # "Chrom composer - from chromos"
            ChromocRes : State (
                _@ <  {
                    = "CHR2"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrChrc (
                    Inp ~ : State {
                        = "CHR2 '{ Node : TestNode; }'"
                    }
                    Inp ~ : State {
                        = "CHR2 '{ Node2 : TestNode; }'"
                    }
                )
            )
            # "Append of string"
            StrApndRes : State (
                _@ <  {
                    = "SS"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrApndVar (
                    Inp1 ~ : State {
                        = "SS Part1_"
                    }
                    Inp2 ~ : State {
                        = "SS Part2"
                    }
                )
            )
            # "Append of URI"
            UriApndRes : State (
                _@ <  {
                    = "URI"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrApndVar (
                    Inp1 ~ : State {
                        = "URI Node1.Node2"
                    }
                    Inp2 ~ : State {
                        = "URI Node3"
                    }
                )
            )
            # "Conversion int to string"
            IntToStr : State (
                _@ <  {
                    = "SS"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrTostrVar (
                    Inp ~ : State {
                        = "SI 34"
                    }
                )
            )
            # "Conversion URI to string"
            SUriToStr : State (
                _@ <  {
                    = "SS 0"
                    LogLevel = "Dbg"
                }
                Inp ~ : TrTostrVar (
                    Inp ~ : State {
                        = "URI bla1.bla2.bla3"
                    }
                )
            )
            # "Conversion URI to string"
            SUriToStr2 : State (
                _@ <  {
                    = "SS 0"
                    LogLevel = "Dbg"
                }
                Inp ~ UriToStr2 : TrTostrVar (
                    Inp ~ : TrApndVar (
                        Inp1 ~ : State {
                            = "URI part1"
                        }
                        Inp2 ~ : State {
                            = "URI part2"
                        }
                    )
                )
            )
            # "Mutation Content"
            SMutCont2 : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "CHR2"
                }
                Inp ~ : TrChr (
                    Mut ~ : TrMutCont (
                        Target ~ : State {
                            = "SS SModelUri"
                        }
                        Name ~ : State {
                            = "SS ''"
                        }
                        Value ~ : State {
                            = "SS test.elem1"
                        }
                    )
                )
            )
            # "Mutation Connect"
            SMutConn : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "MUT"
                }
                Inp ~ : TrMutConn (
                    Cp1 ~ : State {
                        = "SS Cp1"
                    }
                    Cp2 ~ : State {
                        = "SS Cp2"
                    }
                )
            )
            # "Mutation Disonnect"
            SMutDisconn : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "MUT"
                }
                Inp ~ : TrMutDisconn (
                    Cp1 ~ : State {
                        = "SS Cp1"
                    }
                    Cp2 ~ : State {
                        = "SS Cp2"
                    }
                )
            )
            # "Validity indication"
            SIsValid1 : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB "
                }
                Inp ~ : TrIsValid (
                    Inp ~ : State {
                        = "URI "
                    }
                )
            )
            SIsValid2 : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SB "
                }
                Inp ~ : TrIsValid (
                    Inp ~ : State {
                        = "URI hello"
                    }
                )
            )
            # "Select valid URI"
            SSelValid1 : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "URI"
                }
                Inp ~ TSv1 : TrSvldVar (
                    Inp1 ~ : State {
                        = "URI"
                    }
                    Inp2 ~ : State {
                        = "URI Hello"
                    }
                )
            )
            SSelValid2 : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "URI Valid"
                }
                Inp ~ : TrSvldVar (
                    Inp1 ~ : State {
                        = "URI Hello"
                    }
                    Inp2 ~ : State {
                        = "URI World"
                    }
                )
            )
            # "Select valid string"
            SSelValidS1 : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SS "
                }
                Inp ~ : TrSvldVar (
                    Inp1 ~ : State {
                        = "SS "
                    }
                    Inp2 ~ : State {
                        = "SS Hello"
                    }
                )
            )
            SSelValidS2 : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SS Valid"
                }
                Inp ~ : TrSvldVar (
                    Inp1 ~ : State {
                        = "SS Hello"
                    }
                    Inp2 ~ : State {
                        = "SS World"
                    }
                )
            )
            # "URI tail"
            SUriTail : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "URI "
                }
                Inp ~ : TrTailVar (
                    Inp ~ : State {
                        = "URI elem1.elem2.elem3"
                    }
                    Head ~ : State {
                        = "URI elem1"
                    }
                )
            )
            # "URI head"
            SUriHead : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "URI "
                }
                Inp ~ : TrHeadVar (
                    Inp ~ : State {
                        = "URI elem1.elem2.elem3"
                    }
                    Tail ~ : State {
                        = "URI elem3"
                    }
                )
            )
            # "URI tail as num of elems"
            SUriTailn : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "URI "
                }
                Inp ~ : TrTailnVar (
                    Inp ~ : State {
                        = "URI elem1.elem2.elem3"
                    }
                    Num ~ : State {
                        = "SI 1"
                    }
                )
            )
            # "Hash of int"
            SHashInt : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SI"
                }
                Inp ~ : TrHash (
                    Inp ~ : State {
                        = "SI 23"
                    }
                    Inp ~ : State {
                        = "SI 17"
                    }
                )
            )
            # "Hash of string"
            SHashStr : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SI"
                }
                Inp ~ : TrHash (
                    Inp ~ : State {
                        = "SS 'Test string 1'"
                    }
                    Inp ~ : State {
                        = "SS 'Test string 2'"
                    }
                )
            )
            # "Hash of tuple"
            SHashTuple : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SI "
                }
                Inp ~ : TrHash (
                    Inp ~ : State {
                        = "TPL,SS:name,SI:value first 1"
                    }
                    Inp ~ : State {
                        = "TPL,SS:name,SI:value second 2"
                    }
                )
            )
            # "Pair composition"
            SPair1 : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "PSI (SI 1, SI 2)"
                }
                Inp ~ : TrPair (
                    First ~ : State {
                        = "SI 11"
                    }
                    Second ~ : State {
                        = "SI 22"
                    }
                )
            )
            # "Pair compostion then selection"
            SPair2 : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "SI "
                }
                Inp ~ Pair2At : TrAtgVar (
                    Inp ~ : TrPair (
                        First ~ : State {
                            = "SI 11"
                        }
                        Second ~ : State {
                            = "SI 22"
                        }
                    )
                    Index ~ : State {
                        = "SI 0"
                    }
                )
            )
            _ <  {
                # "<<< End Hidden"
            }
        }
    }
}
