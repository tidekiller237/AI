//! \file QueueList_TODO.h
//! \brief Defines the <code>fullsail_ai::data_structures::QueueList</code> class template
//! implementation.
//! \author Cromwell D. Enage
#ifndef _FULLSAIL_AI_FUNDAMENTALS_QUEUELIST_TODO_H_
#define _FULLSAIL_AI_FUNDAMENTALS_QUEUELIST_TODO_H_

namespace fullsail_ai { namespace fundamentals {

	//! \TODO
	//!   - Complete the definition of the <code>QueueList::Iterator</code> pre-increment operator.
	//!   - Complete the definition of the <code>QueueList::Iterator</code> dereference operator.
	//!   - Define the <code>QueueList</code> constructor.
	//!   - Define the <code>QueueList</code> destructor.
	//!   - Define the <code>QueueList</code> methods.


	template <typename T>
	typename QueueList<T>::Iterator& QueueList<T>::Iterator::operator++()
	{
		if (currentNode->next == nullptr)
			currentNode = NULL;
		else
			currentNode = currentNode->next;

		return *this;
	}

	template <typename T>
	T QueueList<T>::Iterator::operator*() const
	{
		return currentNode->data;
	}

	template <typename T>
	QueueList<T>::QueueList() {
		//not sure what to do here honestly.
		front = nullptr;
		back = nullptr;
	}

	template <typename T>
	QueueList<T>::~QueueList() {
		removeAll();
	}

	template <typename T>
	bool QueueList<T>::isEmpty() const {
		return front == nullptr;
	}

	template <typename T>
	void QueueList<T>::enqueue(T element) {
		Node* newNode = new Node(element);

		if (isEmpty()) {
			front = newNode;
			back = newNode;
		}
		else {
			back->next = newNode;
			back = back->next;
		}
	}

	template <typename T>
	T QueueList<T>::getFront() const {
		return front->data;
	}

	template <typename T>
	void QueueList<T>::dequeue() {
		if (front == back) {
			front = nullptr;
			back = nullptr;
		}
		else {
			front = front->next;
		}
	}

	template <typename T>
	void QueueList<T>::removeAll() {
		while (!isEmpty())
			dequeue();
	}

	template <typename T>
	bool QueueList<T>::contains(T element) const {
		Node* curr = front;

		while (curr != nullptr) {
			if (curr->data == element)
				return true;

			curr = curr->next;
		}

		return false;
	}

	template <typename T>
	void QueueList<T>::remove(T element) {
		Node* prev = nullptr;
		Node* curr = front;

		while (curr != nullptr) {
			if (curr->data == element) {
				if (curr == front)
					dequeue();
				else {
					if (curr->next == nullptr)
						back = prev;

					prev->next = curr->next;
				}
				return;
			}

			prev = curr;
			curr = curr->next;
		}
	}

}}  // namespace fullsail_ai::fundamentals

#endif  // _FULLSAIL_AI_FUNDAMENTALS_QUEUELIST_TODO_H_

