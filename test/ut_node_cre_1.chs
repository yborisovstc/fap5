MyRoot : Elem {
    n1 : Node {
        n1_1 : Node
        n1_1 < n1_1_1 : Node
        About = "Test node 1"
        c11 = "Hello"
        LogLevel = "Dbg"
    }
    n1.n1_1 < n1_1_2 : Node {
        n1_1_2_1 : Node
    }
    n1.n1_1 < ! n1_1_1
    n2 : Node {
        n2_1 : Node
    }
    n2 <  {
        n2_2 : Node
    }
    _ <  {
        # "Special target - nil"
    }
}
