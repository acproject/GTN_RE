#ifndef AUTOGRAD_H
#define AUTOGRAD_H

#include "graph.h"
namespace gtn {
void backward(Graph g, const Graph& grad , bool retainGraph = false);
void backward(Graph g, bool retainGraph = false);
}


#endif // AUTOGRAD_H
