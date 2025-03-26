DesUtils : Elem {
    # "DES utilities"
    BChange : Des {
        # "Boolean change"
        SInp : ExtdStateInp
        Outp : ExtdStateOutp
        TAnd : TrAndVar (
            Inp ~ SInp.Int
            Inp ~ : TrNegVar (
                Inp ~ Delay : State (
                    _@ < = "SB false"
                    _@ < LogLevel = "Dbg"
                    Inp ~ SInp.Int
                )
            )
        )
        Outp.Int ~ TAnd
    }
    BChangeCnt : Des {
        # "Boolean change counter"
        SInp : ExtdStateInp
        Outp : ExtdStateOutp
        Chg : BChange
        Chg.SInp ~ SInp.Int
        Cnt : State (
            _@ < = "SI 0"
            _@ < LogLevel = "Dbg"
            Inp ~ : TrAddVar (
                Inp ~ Cnt
                Inp ~ : TrSwitchBool (
                    Sel ~ Chg.Outp
                    Inp1 ~ : SI_0
                    Inp2 ~ : SI_1
                )
            )
        )
        Outp.Int ~ Cnt
    }
    SetTg : Des {
        # "Set trigger"
        InpSet : ExtdStateInp
        Outp : ExtdStateOutp
        Outp.Int ~ Value : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SB false"
            }
            Inp ~ : TrOrVar (
                Inp ~ InpSet.Int
                Inp ~ Value
            )
        )
    }
    RSTg : Des {
        # "R/S trigger, positive inputs"
        InpS : ExtdStateInp
        InpR : ExtdStateInp
        Outp : ExtdStateOutp
        Outp.Int ~ Value : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SB false"
            }
            Inp ~ : TrOrVar (
                Inp ~ InpS.Int
                Inp ~ : TrAndVar (
                    Inp ~ : TrNegVar (
                        Inp ~ InpR.Int
                    )
                    Inp ~ Value
                )
            )
        )
    }
    DPulse : Des {
        # "Data pulse"
        # "Set delay and const type before usage"
        # "InpD - data input, InpE - data to pass when no changes happen"
        InpD : ExtdStateInp
        InpE : ExtdStateInp
        Outp : ExtdStateOutp
        Outp.Int ~ : TrSwitchBool (
            Sel ~ Cmp_Neq : TrCmpVar (
                Inp ~ InpD.Int
                Inp2 ~ Delay : State (
                    Inp ~ InpD.Int
                )
            )
            Inp1 ~ InpE.Int
            Inp2 ~ InpD.Int
        )
    }
    IdxItr : Des {
        # "Index based iterator"
        # "InpCnt - container elements count"
        # "InpDone - sign of selected input is handled"
        # "OutpDone - sign of iterator reaches the end"
        InpCnt : ExtdStateInp
        InpDone : ExtdStateInp
        InpReset : ExtdStateInp
        Outp : ExtdStateOutp
        OutpDone : ExtdStateOutp
        ICnt_Dbg : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SI _INV"
            }
            Inp ~ InpCnt.Int
        )
        SIdx : State (
            # "Index"
            _@ <  {
                = "SI 0"
                LogLevel = "Dbg"
            }
            Inp ~ Sw1 : TrSwitchBool (
                Inp1 ~ Sw2 : TrSwitchBool (
                    Sel ~ CidxAnd1 : TrAndVar (
                        Inp ~ Cmp_Gt : TrCmpVar (
                            Inp ~ InpCnt.Int
                            Inp2 ~ SIdx
                        )
                        Inp ~ InpDone.Int
                    )
                    Inp1 ~ SIdx
                    Inp2 ~ Incr : TrAdd2Var (
                        Inp ~ SIdx
                        Inp2 ~ : SI_1
                    )
                )
                Inp2 ~ : SI_0
                Sel ~ InpReset.Int
            )
        )
        Outp.Int ~ OutpSw : TrSwitchBool (
            Sel ~ Cmp_Gt
            Inp1 ~ : Const {
                = "SI"
            }
            Inp2 ~ SIdx
        )
        OutpDone.Int ~ : TrNegVar (
            Inp ~ Cmp_Gt
        )
    }
    InpItr : Des {
        # "Inputs iterator"
        # "InpM - multiplexed input"
        # "InpDone - sign of selected input is handled"
        # "OutpDone - sign of iterator reaches the end"
        InpM : ExtdStateInp
        InpDone : ExtdStateInp
        InpReset : ExtdStateInp
        Outp : ExtdStateOutp
        OutpDone : ExtdStateOutp
        Itr : IdxItr (
            InpCnt ~ : TrInpCnt (
                Inp ~ InpM.Int
            )
        )
        Itr.InpDone ~ InpDone.Int
        Itr.InpReset ~ InpReset.Int
        Outp.Int ~ Itr.Outp
        OutpDone.Int ~ Itr.OutpDone
    }
    VectIter : IdxItr {
        # "Vector iterator"
        # "InpV - input (vector)"
        # "OutV - output - vector element"
        InpV : ExtdStateInp
        InpCnt ~ : TrSizeVar (
            Inp ~ InpV.Int
        )
        OutV : TrAtgVar (
            Inp ~ InpV.Int
            Index ~ Outp
        )
    }
    # "Data change detector, ref ds_dcs_iui_dci"
    ChgDetector : Des {
        Inp : ExtdStateInp
        Outp : ExtdStateOutp
        Outp.Int ~ Cmp_Neq : TrCmpVar (
            Cmp_Neq.Inp ~ StInpSig : State (
                _@ <  {
                    LogLevel = "Err"
                    = "SI _INV"
                }
                StInpSig.Inp ~ Hash : TrHash (
                    Hash.Inp ~ Inp.Int
                )
            )
            Cmp_Neq.Inp2 ~ Hash
        )
    }
    ListItemByPos : DesAs {
        # "DES active subsystem. Getting list node with given pos in the list"
        # "Input: node position in the list"
        InpPos : ExtdStateInp
        # "Input: link to observed list"
        InpMagLink : Link {
            Outp : CpStateMnodeOutp
        }
        # "Output: URI of list node with given pos"
        OutpNode : ExtdStateOutp
        LogLevel = "Err"
        Subsys : DAdp {
            InpPos : ExtdStateInp
            # "Desas init"
            Init : State {
                = "SB false"
                LogLevel = "Err"
            }
            CurPos : State (
                _@ <  {
                    LogLevel = "Err"
                    = "SI 0"
                }
                Inp ~ : TrSwitchBool (
                    Inp1 ~ : TrSwitchBool (
                        Inp1 ~ : TrAddVar (
                            Inp ~ CurPos
                            Inp ~ : State {
                                = "SI 1"
                            }
                        )
                        Inp2 ~ CurPos
                        Sel ~ PosReached_Ge : TrCmpVar (
                            Inp ~ CurPos
                            Inp2 ~ InpPos.Int
                        )
                    )
                    Inp2 ~ : State {
                        = "SI 0"
                    }
                    Sel ~ Init
                )
            )
            Res : State {
                = "URI _INV"
                LogLevel = "Err"
            }
            PairOfPrev : SdoTcPair (
                Targ ~ Res
                TargComp ~ : State {
                    = "URI Prev"
                }
            )
            PairOfPrev_Dbg : State (
                _@ <  {
                    LogLevel = "Err"
                    = "URI _INV"
                }
                Inp ~ PairOfPrev
            )
            PairOfPrevOwner : SdoCompOwner (
                Comp ~ PairOfPrev
            )
            Res.Inp ~ : TrSwitchBool (
                Inp1 ~ : TrSwitchBool (
                    Inp1 ~ PairOfPrevOwner
                    Inp2 ~ Res
                    Sel ~ PosReached_Ge
                )
                Inp2 ~ : State {
                    = "URI Start"
                }
                Sel ~ Init
            )
        }
        Subsys.InpPos ~ InpPos.Int
        Subsys.InpMagBase ~ InpMagLink.Outp
        Subsys.InpMagUri ~ : State {
            = "URI _$"
        }
        OutpNode.Int ~ Subsys.Res
        # "<<< DES active subsystem. Getting node with given pos of list"
    }
    # "TODO move to specific utils?"
    PrntMappingResolver : DesAs {
        # ">>> Parent mapping resolver"
        # "Finds data (URI) assosiated to parent"
        # "Input: parents hierarchy (VDU)"
        InpParents : ExtdStateInp
        # "Input: Mapping parent to result"
        InpMpg : ExtdStateInp
        # "Input: Default result"
        InpDefRes : ExtdStateInp
        # "Output: Resolved CRP URI"
        OutpRes : ExtdStateOutp
        Subsys : Des {
            InpSsParents : ExtdStateInp
            InpMapping : ExtdStateInp
            Outp : ExtdStateOutp
            # "Desas init indicator"
            Init : State {
                = "SB false"
                LogLevel = "Err"
            }
            ParentsIter : VectIter (
                _@ < LogLevel = "Dbg"
                InpV ~ InpSsParents.Int
                _ < InpDone ~ : SB_True
                InpReset ~ Init
            )
            FindCrp : TrFindByP (
                Inp ~ InpMapping.Int
                Sample ~ ParentsIter.OutV
            )
            ParentsIter.InpDone ~ : TrNegVar (
                Inp ~ : TrIsValid (
                    Inp ~ FindCrp
                )
            )
            Outp.Int ~ FindCrp
        }
        Parents_Dbg : State (
            _@ < LogLevel = "Dbg"
            _@ < = "VDU"
            Inp ~ InpParents.Int
        )
        Res_Dbg : State (
            _@ < LogLevel = "Dbg"
            _@ < = "URI"
            Inp ~ Subsys.Outp
        )
        NotFound : TrAndVar (
            Inp ~ : TrIsValid (
                Inp ~ InpMpg.Int
            )
            Inp ~ : TrIsValid (
                Inp ~ InpParents.Int
            )
            Inp ~ : TrIsInvalid (
                Inp ~ Subsys.Outp
            )
        )
        Subsys.InpSsParents ~ InpParents.Int
        Subsys.InpMapping ~ InpMpg.Int
        _ <  {
            OutpRes.Int ~ Subsys.Outp
            OutpRes.Int ~ OutpRes_Int : TrSvldVar (
                _@ < LogLevel = "Dbg"
                Inp1 ~ Subsys.Outp
                Inp2 ~ InpDefRes.Int
            )
        }
        OutpRes.Int ~ OutpRes_Int : TrSwitchBool (
            _@ < LogLevel = "Dbg"
            Inp1 ~ Subsys.Outp
            Inp2 ~ InpDefRes.Int
            Sel ~ NotFound
        )
        # ">>> Parent mapping resolver"
    }
    PrntMappingResolver2 : DesAs2 {
        # ">>> Parent mapping resolver, v.2 based on DesAs2, not working atm"
        # "Finds data (URI) assosiated to parent"
        # "Input: parents hierarchy (VDU)"
        InpParents : ExtdStateInp
        # "Input: Mapping parent to result"
        InpMpg : ExtdStateInp
        # "Input: Default result"
        InpDefRes : ExtdStateInp
        # "Output: Resolved CRP URI, buf and CP"
        OutpRes : State {
            LogLevel = "Dbg"
            = "URI"
        }
        Subsys : Des {
            InpSsParents : ExtdStateInp
            InpMapping : ExtdStateInp
            Outp : ExtdStateOutp
            # "Desas init indicator"
            Init : State {
                = "SB false"
                LogLevel = "Err"
            }
            ParentsIter : VectIter (
                _@ < LogLevel = "Dbg"
                InpV ~ InpSsParents.Int
                _ < InpDone ~ : SB_True
                InpReset ~ Init
            )
            FindCrp : TrFindByP (
                Inp ~ InpMapping.Int
                Sample ~ ParentsIter.OutV
            )
            ParentsIter.InpDone ~ : TrNegVar (
                Inp ~ : TrIsValid (
                    Inp ~ FindCrp
                )
            )
            Outp.Int ~ FindCrp
        }
        Parents_Dbg : State (
            _@ < LogLevel = "Dbg"
            _@ < = "VDU"
            Inp ~ InpParents.Int
        )
        Res_Dbg : State (
            _@ < LogLevel = "Dbg"
            _@ < = "URI"
            Inp ~ Subsys.Outp
        )
        NotFound : TrAndVar (
            Inp ~ : TrIsValid (
                Inp ~ InpMpg.Int
            )
            Inp ~ : TrIsValid (
                Inp ~ InpParents.Int
            )
            Inp ~ : TrIsInvalid (
                Inp ~ Subsys.Outp
            )
        )
        Subsys.InpSsParents ~ InpParents.Int
        Subsys.InpMapping ~ InpMpg.Int
        OutpRes.Inp ~ OutpRes_Int : TrSwitchBool (
            Inp1 ~ Subsys.Outp
            Inp2 ~ InpDefRes.Int
            Sel ~ NotFound
        )
        # ">>> Parent mapping resolver"
    }
}
