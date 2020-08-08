namespace TSnap {

/////////////////////////////////////////////////
// Graph Algorithms

// RenumberNodes ... Renumber node ids in the subgraph to 0...N-1
PUNGraph GetSubGraph(const PUNGraph& Graph, const TIntV& NIdV, const bool& RenumberNodes) {
  //if (! RenumberNodes) { return TSnap::GetSubGraph(Graph, NIdV); }
  PUNGraph NewGraphPt = TUNGraph::New();
  TUNGraph& NewGraph = *NewGraphPt;
  NewGraph.Reserve(NIdV.Len(), -1);
  TIntSet NIdSet(NIdV.Len());
  for (int n = 0; n < NIdV.Len(); n++) {
    if (Graph->IsNode(NIdV[n])) {
      NIdSet.AddKey(NIdV[n]);
      if (! RenumberNodes) { NewGraph.AddNode(NIdV[n]); }
      else { NewGraph.AddNode(NIdSet.GetKeyId(NIdV[n])); }
    }
  }
  if (! RenumberNodes) {
    for (int n = 0; n < NIdSet.Len(); n++) {
      const int SrcNId = NIdSet[n];
      const TUNGraph::TNodeI NI = Graph->GetNI(SrcNId);
      for (int edge = 0; edge < NI.GetOutDeg(); edge++) {
        const int OutNId = NI.GetOutNId(edge);
        if (NIdSet.IsKey(OutNId)) {
          NewGraph.AddEdge(SrcNId, OutNId); }
      }
    }
  } else {
    for (int n = 0; n < NIdSet.Len(); n++) {
      const int SrcNId = NIdSet[n];
      const TUNGraph::TNodeI NI = Graph->GetNI(SrcNId);
      for (int edge = 0; edge < NI.GetOutDeg(); edge++) {
        const int OutNId = NI.GetOutNId(edge);
        if (NIdSet.IsKey(OutNId)) {
          NewGraph.AddEdge(NIdSet.GetKeyId(SrcNId), NIdSet.GetKeyId(OutNId)); }
      }
    }
  }
  return NewGraphPt;
}

// RenumberNodes ... Renumber node ids in the subgraph to 0...N-1
PNGraph GetSubGraph(const PNGraph& Graph, const TIntV& NIdV, const bool& RenumberNodes) {
  //if (! RenumberNodes) { return TSnap::GetSubGraph(Graph, NIdV); }
  PNGraph NewGraphPt = TNGraph::New();
  TNGraph& NewGraph = *NewGraphPt;
  NewGraph.Reserve(NIdV.Len(), -1);
  TIntSet NIdSet(NIdV.Len());
  for (int n = 0; n < NIdV.Len(); n++) {
    if (Graph->IsNode(NIdV[n])) {
      NIdSet.AddKey(NIdV[n]);
      if (! RenumberNodes) { NewGraph.AddNode(NIdV[n]); }
      else { NewGraph.AddNode(NIdSet.GetKeyId(NIdV[n])); }
    }
  }
  if (! RenumberNodes) {
    for (int n = 0; n < NIdSet.Len(); n++) {
      const int SrcNId = NIdSet[n];
      const TNGraph::TNodeI NI = Graph->GetNI(SrcNId);
      for (int edge = 0; edge < NI.GetOutDeg(); edge++) {
        const int OutNId = NI.GetOutNId(edge);
        if (NIdSet.IsKey(OutNId)) {
          NewGraph.AddEdge(SrcNId, OutNId); }
      }
    }
  } else {
    for (int n = 0; n < NIdSet.Len(); n++) {
      const int SrcNId = NIdSet[n];
      const TNGraph::TNodeI NI = Graph->GetNI(SrcNId);
      for (int edge = 0; edge < NI.GetOutDeg(); edge++) {
        const int OutNId = NI.GetOutNId(edge);
        if (NIdSet.IsKey(OutNId)) {
          NewGraph.AddEdge(NIdSet.GetKeyId(SrcNId), NIdSet.GetKeyId(OutNId)); }
      }
    }
  }
  return NewGraphPt;
}

PUNGraph GetEgonet(const PUNGraph& Graph, const int CtrNId, int& ArndEdges) {
  PUNGraph NewGraphPt = TUNGraph::New();
  TUNGraph& NewGraph = *NewGraphPt;
  NewGraph.AddNode(CtrNId);
  const TUNGraph::TNodeI& CtrNode = Graph->GetNI(CtrNId);
  for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
    NewGraph.AddNode(CtrNode.GetInNId(i));
  }
  ArndEdges = 0;
  for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
    int NbrNId = CtrNode.GetInNId(i);
    const TUNGraph::TNodeI& NbrNode = Graph->GetNI(NbrNId);
    for (int j = 0; j < NbrNode.GetInDeg(); ++j) {
      int NbrNbrNId = NbrNode.GetInNId(j);
      if (NewGraph.IsNode(NbrNbrNId)) {
        if (!NewGraph.IsEdge(NbrNId, NbrNbrNId)) {
          NewGraph.AddEdge(NbrNId, NbrNbrNId);
        }
      } else {
        ArndEdges++;
      }
    }
  }
  return NewGraphPt;
}

PNGraph GetEgonet(const PNGraph& Graph, const int CtrNId, int& InEdges, int& OutEdges) {
  PNGraph NewGraphPt = TNGraph::New();
  TNGraph& NewGraph = *NewGraphPt;
  NewGraph.AddNode(CtrNId);
  const TNGraph::TNodeI& CtrNode = Graph->GetNI(CtrNId);
  for (int i = 0; i < CtrNode.GetDeg(); ++i) {
    if (!NewGraph.IsNode(CtrNode.GetNbrNId(i))) {
      NewGraph.AddNode(CtrNode.GetNbrNId(i));
    }
  }
  InEdges = 0;
  OutEdges = 0;
  for (int i = 0; i < CtrNode.GetDeg(); ++i) {
    int NbrNId = CtrNode.GetNbrNId(i);
    const TNGraph::TNodeI& NbrNode = Graph->GetNI(NbrNId);
    for (int j = 0; j < NbrNode.GetInDeg(); ++j) {
      int NbrNbrNId = NbrNode.GetInNId(j);
      if (NewGraph.IsNode(NbrNbrNId)) {
        NewGraph.AddEdge(NbrNbrNId, NbrNId);
      } else {
        InEdges++;
      }
    }
    for (int j = 0; j < NbrNode.GetOutDeg(); ++j) {
      int NbrNbrNId = NbrNode.GetOutNId(j);
      if (NewGraph.IsNode(NbrNbrNId)) {
        NewGraph.AddEdge(NbrNId, NbrNbrNId);
      } else {
        OutEdges++;
      }
    }
  }
  return NewGraphPt;
}

PUNGraph GetEgonetHop(const PUNGraph &Graph, const int CtrNId, const int Radius) {
  PUNGraph NewGraphPt = TUNGraph::New();
  TUNGraph &NewGraph = *NewGraphPt;
  TSnapQueue<int> Queue1;
  TSnapQueue<int> Queue2;
  Queue1.Clr(false); Queue1.Push(CtrNId);
  for (int r = 0; r < Radius; ++r) {
    Queue2.Clr(true);
    while (! Queue1.Empty()) {
      const int NId = Queue1.Top(); Queue1.Pop();
      NewGraph.AddNode(NId);
      const TUNGraph::TNodeI &Node = Graph->GetNI(NId);
      for (int i = 0; i < Node.GetInDeg(); ++i)
      {
        const int InNId = Node.GetInNId(i);
        if (! NewGraph.IsNode(InNId))
        {
          NewGraph.AddNode(InNId);
          Queue2.Push(InNId);
        }
      }
      for (int i = 0; i < Node.GetInDeg(); ++i)
      {
        int NbrNId = Node.GetInNId(i);
        const TUNGraph::TNodeI &NbrNode = Graph->GetNI(NbrNId);
        for (int j = 0; j < NbrNode.GetInDeg(); ++j)
        {
          int NbrNbrNId = NbrNode.GetInNId(j);
          if (NewGraph.IsNode(NbrNbrNId))
          {
            if (!NewGraph.IsEdge(NbrNId, NbrNbrNId))
            {
              NewGraph.AddEdge(NbrNId, NbrNbrNId);
            }
          }
        }
      }
    }
    Queue1 = Queue2;
  }
  return NewGraphPt;
}

} // namespace TSnap
