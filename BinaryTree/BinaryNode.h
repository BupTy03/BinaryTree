#pragma once
#ifndef BINARYNODE_H
#define BINARYNODE_H

#include<iostream>
#include<memory>
#include<queue>
#include<cassert>

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
			if (left_ != nullptr)
			{
				alloc_.destroy(left_);
				alloc_.deallocate(left_, 1);
			}
			if (right_ != nullptr)
			{
				alloc_.destroy(right_);
				alloc_.deallocate(right_, 1);
			}
		}

		void addNode(BinaryNode* child)
		{
			if (child == nullptr) return;
			if (child->data_ < this->data_)
			{
				if (this->left_ == nullptr)
				{
					child->parent_ = this;
					this->left_ = child;
				}
				else
					this->left_->addNode(child);
			}
			else
			{
				if (this->right_ == nullptr)
				{
					child->parent_ = this;
					this->right_ = child;
				}
				else
					this->right_->addNode(child);
			}
		}
		void addNode(const T& new_data)
		{ 
			BinaryNode* node = alloc_.allocate(1);
			alloc_.construct(node, new_data);

			addNode(node);
		}
		void addNode(T&& new_data)
		{ 
			BinaryNode* node = alloc_.allocate(1);
			alloc_.construct(node, std::move(new_data));

			addNode(node);
		}

		BinaryNode* getLeftChild() { return left_; }
		BinaryNode* getRightChild() { return right_; }

		const BinaryNode* getLeftChild() const { return left_; }
		const BinaryNode* getRightChild() const { return right_; }

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
		const BinaryNode* findNode(const T& key) const
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
		void setData(T&& data) { data_ = std::move(data); }

		BinaryNode* clone() const { return new BinaryNode(data_); }

		void deleteNode(const T& data)
		{
			BinaryNode* node = findNode(data);
			if (node == nullptr) return;

			// first situation
			if (node->left_ == nullptr && node->right_ == nullptr)
			{
				setup_new_node(node, nullptr);
				deallocateNode(node);
				return;
			}

			// second situation
			if (node->left_ != nullptr && node->right_ != nullptr)
			{
				if (node->left_->right_ == nullptr)
				{
					setup_new_node(node, node->left_);
					node->left_->right_ = node->right_;
				}
				else
				{
					BinaryNode* tmp = node->left_->right_;
					while (tmp->right_ != nullptr) tmp = tmp->right_;

					if (tmp->left_ != nullptr)
					{
						tmp->parent_->right_ = tmp->left_;
						tmp->left_->parent_ = tmp->parent_;
					}
					setup_new_node(node, tmp);
					node->left_->right_ = nullptr;
					tmp->left_ = node->left_;
					tmp->right_ = node->right_;
				}

				deallocateNode(node);
				return;
			}

			// third situation
			if (node->left_ != nullptr)
				setup_new_node(node, node->left_);
			else
				setup_new_node(node, node->right_);

			deallocateNode(node);
		}

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
		void setup_new_node(BinaryNode* old_node, BinaryNode* new_node)
		{
			assert(old_node != nullptr);

			if (old_node->parent_ == nullptr)
				throw std::runtime_error{ "it's impossible to delete a root!" };

			if (old_node->parent_->left_ == old_node)
				old_node->parent_->left_ = new_node;
			else
				old_node->parent_->right_ = new_node;

			if (new_node == nullptr) return;

			new_node->parent_ = old_node->parent_;
		}
		void deallocateNode(BinaryNode* node)
		{
			node->left_ = nullptr;
			node->right_ = nullptr;
			alloc_.destroy(node);
			alloc_.deallocate(node, 1);
		}

		BinaryNode* parent_{};
		BinaryNode* left_{};
		BinaryNode* right_{};
		allocator_type alloc_;
		T data_;
	};
}

#endif // !BINARYNODE_H