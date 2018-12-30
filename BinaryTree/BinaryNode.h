#pragma once
#ifndef BINARYNODE_HPP
#define BINARYNODE_HPP

#include<memory>
#include<queue>

namespace my
{
	template<class T, class A = std::allocator<T>>
	class BinaryNode
	{
	public:
		using value_type = T;
		using allocator_type = typename std::allocator_traits<A>::template rebind_alloc<BinaryNode>;

		BinaryNode() {}
		BinaryNode(const T& data) : data_{ data } {}
		BinaryNode(T&& data) : data_{ std::move(data) } {}
		~BinaryNode()
		{
			delete left_;
			delete right_;
		}

		void addLeftChild(BinaryNode* child)
		{
			if (child == nullptr) return;
			left_ = child;
		}
		void addRightChild(BinaryNode* child)
		{
			if (child == nullptr) return;
			right_ = child;
		}
		void addNode(BinaryNode* child)
		{
			if (child == nullptr) return;
			if (child->data_ < this->data_)
			{
				if (this->left_ == nullptr)
					this->left_ = child;
				else
					this->left_->addNode(child);
			}
			else
			{
				if (this->right_ == nullptr)
					this->right_ = child;
				else
					this->right_->addNode(child);
			}
		}
		void addNode(const T& new_data)
		{ this->addNode(new BinaryNode(new_data)); }
		void addNode(T&& new_data)
		{ this->addNode(new BinaryNode(std::move(new_data))); }

		BinaryNode* findNode(const T& key)
		{
			if (this->data_ == key) return this;
			if (key < this->data_)
			{
				if (this->left_ == nullptr) return nullptr;
				return this->left_->findNode(key);
			}
			else
			{
				if (this->right_ == nullptr) return nullptr;
				return this->right_->findNode(key);
			}
		}

		const T& data() const { return data; }
		T& data() { return data_; }

		void setData(const T& data) { data_ = data; }
		void setData(T&& data) { data_ = data; }

		BinaryNode* clone() { return new BinaryNode(data_); }

		template<typename F, typename... Args>
		friend void TraversePreorder(BinaryNode* node, F func, Args&&... args)
		{
			if (node == nullptr) return;
			func(node->data_, std::forward<Args>(args)...);
			if (node->left_ != nullptr) TraversePreorder(node->left_, func, std::forward<Args>(args)...);
			if (node->right_ != nullptr) TraversePreorder(node->right_, func, std::forward<Args>(args)...);
		}

		template<typename F, typename... Args>
		friend void TraverseInorder(BinaryNode* node, F func, Args&&... args)
		{
			if (node == nullptr) return;
			if (node->left_ != nullptr) TraversePreorder(node->left_, func, std::forward<Args>(args)...);
			func(node->data_, std::forward<Args>(args)...);
			if (node->right_ != nullptr) TraversePreorder(node->right_, func, std::forward<Args>(args)...);
		}

		template<typename F, typename... Args>
		friend void TraversePostorder(BinaryNode* node, F func, Args&&... args)
		{
			if (node == nullptr) return;
			if (node->left_ != nullptr) TraversePreorder(node->left_, func, std::forward<Args>(args)...);
			if (node->right_ != nullptr) TraversePreorder(node->right_, func, std::forward<Args>(args)...);
			func(node->data_, std::forward<Args>(args)...);
		}

		template<typename F, typename... Args>
		friend void TraverseDepthFirst(BinaryNode* node, F func, Args&&... args)
		{
			if (node == nullptr) return;

			std::queue<BinaryNode*> children;
			children.push(node);
			
			while (!children.empty())
			{
				BinaryNode* current = children.front();
				children.pop();

				func(current->data_, std::forward<Args>(args)...);

				if (current->left_ != nullptr) children.push(current->left_);
				if (current->right_ != nullptr) children.push(current->right_);
			}
		}

	private:
		BinaryNode* left_{};
		BinaryNode* right_{};
		T data_;
	};
}

#endif // !BINARYNODE_HPP