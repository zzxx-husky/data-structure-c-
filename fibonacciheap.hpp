template<typename T>
struct FHNode {
	T value;
	int index;
	FHNode<T> *son, *slibling;

	FHNode(const T & v):value(v), index(0), son(nullptr), slibling(nullptr) {}

	FHNode<T> * merge(FHNode<T> * b) {
		b->slibling = son;
		son = b;
		index ++;
		return this;
	}
};

template<typename T>
class FibonacciHeap {
	FHNode<T> * heaps[32];
	int maxIdx;
	FHNode<T> *tmp, *topNode;

	bool addNodeTo(int i) {
		if (heaps[i] == nullptr) {
			heaps[i] = tmp;
			return true;
		}
		if (heaps[i]->value < tmp->value) 
			tmp = heaps[i]->merge(tmp);
		else
			tmp = tmp->merge(heaps[i]);
		heaps[i] = nullptr;
		return false;
	}

	void remove(FHNode<T> * node) {
		if (node->slibling != nullptr)
			remove(node->slibling);
		if (node->son != nullptr)
			remove(node->son);
		delete node;
	}
public:
	FibonacciHeap(): maxIdx(0), tmp(nullptr), topNode(nullptr) {
		for (int i = 0;i < 32;i++) heaps[i] = nullptr;
	}

	const T & top() {
		if (topNode == nullptr)
			throw new std::runtime_error("top from an empty fibonacci heap");
		return topNode->value;
	}

	void add(const T & value) {
		tmp = new FHNode<T>(value);
		if (topNode == nullptr || value <= topNode->value)
			topNode = tmp;
		for (int i = 0;i <= maxIdx;i++)
			if (addNodeTo(i)) break;
		if (heaps[maxIdx] != nullptr)
			maxIdx ++;
		tmp = nullptr;
	}

	T && pop() {
		if (topNode == nullptr)
			throw new std::runtime_error("top from an empty fibonacci heap");

		heaps[topNode->index] = nullptr;
		for (FHNode<T> * son = topNode->son;son != nullptr;) {
			tmp = son;
			son = son->slibling;
			tmp->slibling = nullptr;
			for (int i = tmp->index;i <= maxIdx;i++)
				if (addNodeTo(i)) break;
		}
		tmp = nullptr;
		if (heaps[maxIdx] != nullptr)
			maxIdx ++;

		T res = std::move(topNode->value);
		delete topNode;
		topNode = nullptr;
		for (int i = 0;i < maxIdx;i++) {
			if (heaps[i] != nullptr) {
				if (topNode == nullptr || heaps[i]->value < topNode->value)
					topNode = heaps[i];
			}
		}
		return std::move(res);
	}

	bool empty() {
		return topNode == nullptr;
	}

	~FibonacciHeap() {
		for (int i = 0;i < maxIdx;i++) {
			if (heaps[i] != nullptr) {
				remove(heaps[i]);
			}
		}
	}
};
