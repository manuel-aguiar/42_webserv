/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 08:49:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 09:32:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <deque>
#include <iostream>

class TreeNode
{
public:
    TreeNode(int value) : m_value(value), m_left(NULL), m_right(NULL) {}
    int m_value;
    TreeNode* m_left;
    TreeNode* m_right;
};

void delTree(TreeNode* root)
{
    if (root == NULL)
        return ;
    delTree(root->m_left);
    delTree(root->m_right);
    delete root;
}

void bfs(TreeNode* root)
{
    size_t curSize;
    std::deque<TreeNode*> queue;

    queue.push_back(root);
    curSize = queue.size();

    while (curSize)
    {
        for (size_t i = 0; i < curSize; i++)
        {
            TreeNode* cur = queue.front();
            std::cout << cur->m_value << " ";

            if (cur->m_left)
                queue.push_back(cur->m_left);
            if (cur->m_right)
                queue.push_back(cur->m_right);
            queue.pop_front();
        }
        std::cout << '\n';

        curSize = queue.size();
    }
    std::cout << std::endl;
}

void dfsInternal(TreeNode* root)
{
    if (!root)
        return ;
    dfsInternal(root->m_left);
    dfsInternal(root->m_right);
    std::cout << root->m_value << " ";
}

void dfs(TreeNode* root)
{
    dfsInternal(root);
    std::cout << std::endl;
}

void inOrderInternal(TreeNode* root)
{
    if (!root)
        return ;
    inOrderInternal(root->m_left);
    std::cout << root->m_value << " ";
    inOrderInternal(root->m_right);
}

void inOrder(TreeNode* root)
{
    inOrderInternal(root);
    std::cout << std::endl;
}

void preOrderInternal(TreeNode* root)
{
    if (!root)
        return ;
    std::cout << root->m_value << " ";
    preOrderInternal(root->m_left);
    preOrderInternal(root->m_right);
}

void preOrder(TreeNode* root)
{
    preOrderInternal(root);
    std::cout << std::endl;
}

void postOrderInternal(TreeNode* root)
{
    if (!root)
        return ;
    postOrderInternal(root->m_left);
    postOrderInternal(root->m_right);
    std::cout << root->m_value << " ";
}

void postOrder(TreeNode* root)
{
    postOrderInternal(root);
    std::cout << std::endl;
}

/* c++ -Wall -Wextra -Werror --std=c++98 main.cpp -o test.out && ./test.out */

int main(void)
{
    TreeNode* root = new TreeNode(1);
    root->m_left = new TreeNode(2);
    root->m_right = new TreeNode(3);
    root->m_left->m_left = new TreeNode(4);
    root->m_right->m_left = new TreeNode(5);
    root->m_right->m_right = new TreeNode(6);

    bfs(root);
    dfs(root);
    inOrder(root);
    preOrder(root);
    postOrder(root);
    return 0;
}