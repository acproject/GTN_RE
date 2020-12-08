#ifndef GRAPH_H
#define GRAPH_H

#include <cassert>
#include <climits>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace gtn {
 constexpr int epsilon = -1;

 class Graph
 {
 private:
     struct Node {
         Node(bool start, bool accept): start(start), accept(accept){};
         bool start {false};
         bool accept {false};
         std::vector<int> in;
         std::vector<int> out;
     };

     struct Arc {
         Arc(int srcNode, int dstNode, int ilabel, int olabel)
             :srcNode(srcNode), dstNode(dstNode), ilabel(ilabel), olabel(olabel) {};


         int srcNode;
         int dstNode;
         int ilabel;
         int olabel;
     };



 public:
     using GradFunc = std::function<void(std::vector<Graph>& inputs, Graph& deltas)>;
     Graph(GradFunc gradFunc, std::vector<Graph> inputs);

     Graph(bool calcGrad = true);

     int addNode(bool start = false, bool accept = false);

     size_t addArc(size_t srcNode, size_t dstNode, int label);

     size_t addArc(size_t srcNode, size_t dstNode, int ilabel,
                   int olabel, float weight = 0.0);

     size_t numArcs() const {
         return sharedGraph_->arcs.size();
     }

     size_t numNodes() const {
         return sharedGraph_->nodes.size();
     }

     size_t numStart() const {
         return sharedGraph_->start.size();
     }

     size_t numAccept() const {
         return sharedGraph_->accept.size();
     }

     /**
     * @brief item 从单个邻接图获得权重
     * @return
     */
    float item() const;

    static Graph deepCopy(const Graph& src);

    void arcSort(bool olabel = false);

    void markArcSorted(bool olabel = false) {
        if(olabel) {
            sharedGraph_->olabelSorted = true;
        }else {
            sharedGraph_->ilabelSorted = true;
        }
    }

    bool ilabelSorted() const {
        return sharedGraph_->ilabelSorted;
    }

    bool olabelSorted() const {
        return sharedGraph_->olabelSorted;
    }

    /**
     * @brief setWeights
     * @param weights
     * `Graph::numArcs()` elements.
     */
    void setWeights(const float* weights);

    void labelsToArray(int* out, bool ilabel = true);

    std::vector<int> labelsToVector(bool ilabel = true);

    void addGrad(std::vector<float>&& other);

    void addGrad(const std::vector<float>& other);

    void addGrad(const Graph& other);

    bool calcGrad() const  {
        return sharedGraph_-> calcGrad;
    }

    bool isGradAvilable() const {
        return sharedGraph_->grad != nullptr;
    }

    Graph& grad();

    const Graph& grad() const;

    void setCalcGrad(bool calcGrad);

    void zeroGrad();

    std::uintptr_t id();

    GradFunc gradFunc() {
        return sharedGraph_->gradFunc;
    }

    void setGradFunc(GradFunc gradFunc) {
        if(calcGrad()) {
            sharedGraph_->gradFunc = gradFunc;
        }
    }

    std::vector<Graph>& inputs() {
        return sharedGraph_->inputs;
    }

    Graph withoutWeights() const {
        Graph other = *this;
        other.sharedWeights_ = nullptr;
        return other;
    }

    const std::vector<int>& start() const {
        return sharedGraph_->start;
    }

    const std::vector<int>& accept() const {
        return sharedGraph_->accept;
    }

    bool isStart(size_t i) const {
        return node(i).start;
    }

    bool isAccept(size_t i) const {
        return node(i).accept;
    }

    void makeAccept(size_t i) {
        auto& n = node(i);
        if(!n.accept) {
            sharedGraph_->accept.push_back(static_cast<int>(i));
            n.accept = true;
        }
    }

    size_t numOut(size_t i) const {
        return node(i).out.size();
    }

    const std::vector<int>& out(size_t i) const {
        return node(i).out;
    }

    int out(size_t i, size_t j) const {
        return node(i).out[j];
    }

    size_t numIn(size_t i) const {
        return node(i).in.size();
    }

    const std::vector<int>& in(size_t i) const {
        return node(i).in;
    }

    size_t in(size_t i, size_t j) const {
        return node(i).in[j];
    }

    int srcNode(size_t i) const {
        return arc(i).srcNode;
    }

    int dstNode(size_t i) const {
        return arc(i).dstNode;
    }

    int label(size_t i) const {
        return arc(i).ilabel;
    }

    int ilabel(size_t i) const {
        return arc(i).ilabel;
    }

    int olabel(size_t i) const {
        return arc(i).olabel;
    }

    float weight(size_t i) const {
        assert(sharedWeights_ != nullptr);
        return(*sharedWeights_)[i];
    }

    void setWeight(size_t i, float weight) {
        assert(sharedWeights_ != nullptr);
        (*sharedWeights_)[i] = weight;
    }

 private:
    size_t addArc(size_t srcNode, size_t dstNode, int label, float) = delete;
    size_t addArc(size_t srcNode, size_t dstNode, int label, double) = delete;

    const Node& node(size_t i) const {
        assert(i >= 0 && i < numNodes());
        return sharedGraph_->nodes[i];
    }

    Node& node(size_t i){
        return const_cast<Node&>(static_cast<const Graph&>(*this).node(i));
    }

    const Arc& arc(size_t i) const {
        assert( i >= 0 && i < numArcs());
        return sharedGraph_->arcs[i];
    }

    Arc& arc(size_t i) {
        return const_cast<Arc&>(static_cast<const Arc&>(*this).arc(i));
    }

    struct SharedGraph {
       std::vector<Arc> arcs;
       std::vector<Node> nodes;
       std::vector<int> start;
       std::vector<int> accept;

       bool ilabelSorted{false};
       bool olabelSorted{false};
       // 此对象的线程锁
       std::mutex grad_lock;
    };

    std::shared_ptr<SharedGraph> sharedGraph_{std::make_shared<SharedGraph>()};

    std:;shared_ptr<std::vector<float>> sharedWeights_{
        std::make_shared<std::vector<float>>()
    };
    std::shared_ptr<SharedGrad> sharedGrad_{std::make_shared<SharedGrad>()};
 };
} // namespace gtn


#endif // GRAPH_H
