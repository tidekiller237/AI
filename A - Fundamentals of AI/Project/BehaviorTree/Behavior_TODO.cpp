//! \file Behavior_TODO.cpp
//! \brief Implements the <code>fullsail_ai::fundamentals::Behavior</code> abstract class.
//! \author Jeremiah Blanchard with code from Cromwell D. Enage

// #include <queue>   NOTE: Only use if your QueueList is not working
#include <stack>
#include "Behavior.h"

namespace fullsail_ai { namespace fundamentals {

	// DO NOT Edit or Use these two variables for any reason, it is for memory leak check purposes only.
	unsigned Behavior::createdCount = 0;
	unsigned Behavior::destroyedCount = 0;

	unsigned Behavior::getCreatedCount()
	{
		return createdCount;
	}

	unsigned Behavior::getDestroyedCount()
	{
		return destroyedCount;
	}

	Behavior::Behavior(char const* _description)
	{
		description = _description;
	}

	char const* Behavior::toString() const
	{
		return description;
	}

	// DO NOT edit this function, it is supposed to just return false
	bool Behavior::isLeaf() const
	{
		return false;
	}

	//! \TODO
	//!   - Implement the <code>Behavior</code> methods.
	
	//basic methods
	size_t Behavior::getChildCount() const {
		return children.size();
	}

	Behavior* Behavior::getChild(size_t index) {
		return children[index];
	}

	Behavior const* Behavior::getChild(size_t index) const {
		return children[index];
	}

	void Behavior::addChild(Behavior* child) {
		children.push_back(child);
		child->parent = this;
	}

	//traversal methods
	void Behavior::breadthFirstTraverse(void (*dataFunction)(Behavior const*)) const {
		QueueList<Behavior const*> q;
		q.enqueue(this);

		while (!q.isEmpty()) {
			Behavior const* node = q.getFront();
			q.dequeue();
			dataFunction(node);

			for (int i = 0; i < node->getChildCount(); i++) {
				q.enqueue(node->getChild(i));
			}
		}
	}

	void Behavior::preOrderTraverse(void (*dataFunction)(Behavior const*)) const {
		dataFunction(this);

		for (int i = 0; i < getChildCount(); i++) {
			getChild(i)->preOrderTraverse(dataFunction);
		}
	}

	void Behavior::postOrderTraverse(void (*dataFunction)(Behavior const*)) const {
		for (int i = 0; i < getChildCount(); i++) {
			getChild(i)->postOrderTraverse(dataFunction);
		}

		dataFunction(this);
	}

}}  // namespace fullsail_ai::fundamentals
