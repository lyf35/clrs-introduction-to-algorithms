#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<list>
#include<memory>
#include<queue>
#include<stack>
#include<tuple>
#include"Fibonacci_heap.h"
#include"disjointed_data_set.h"

enum edge_status { tree_edge, backward_edge, forward_edge, cross_edge, undetermined };
enum vertex_status { undiscovered, discovered, visited };
enum graph_type { undirected, directed };

//用于最小路径比较的运算符
bool operator<(std::tuple<int, int, double> a, std::tuple<int, int, double> b)
{
	return std::get<2>(a) < std::get<2>(b);
}
bool operator>(std::tuple<int, int, double> a, std::tuple<int, int, double> b)
{
	return std::get<2>(a) > std::get<2>(b);
}
bool operator==(std::tuple<int, int, double> a, std::tuple<int, int, double> b)
{
	return std::get<2>(a) == std::get<2>(b);
}

template<typename T> struct graph_vertex
{
	int indegree;
	int outdegree;
	int discover_time_dfs; //dt和ft用于dfs的搜索过程，初始化为-1，代表还没有开始搜索
	int finish_time_dfs;
	int discover_time_bfs; //用于bfs的搜索过程，记录它距离开始搜索结点的距离，初始化为-1，代表还没有开始搜索
	int region_bfs; //代表bfs搜索时顶点所在的连通域
	vertex_status stat_bfs;
	vertex_status stat_dfs;
	int parent_bfs; //用于bfs和dfs的搜索过程，若值为-1，则代表bfs在搜索时，它为开始搜索的结点
	int parent_dfs;
	T key;
	graph_vertex(T k, int in = 0, int out = 0, int dtd = -1, int ftd = -1, int dtb = -1, int reg = 0, vertex_status s_bfs = undiscovered, vertex_status s_dfs = undiscovered, int p_b = -1, int p_d = -1) :indegree(in), outdegree(out), key(k), discover_time_dfs(dtd), finish_time_dfs(ftd), discover_time_bfs(dtb), region_bfs(reg), stat_bfs(s_bfs), stat_dfs(s_dfs), parent_bfs(p_b), parent_dfs(p_d) {};
	graph_vertex() = default;
	void init_bfs() //对bfs搜索的数据进行重置
	{
		discover_time_bfs = -1;
		region_bfs = 0;
		stat_bfs = undiscovered;
		parent_bfs = -1;
	}
	void init_dfs() //对dfs搜索的数据进行重置
	{
		discover_time_dfs = -1;
		finish_time_dfs = -1;
		stat_dfs = undiscovered;
		parent_dfs = -1;
	}
};

template<typename W> struct graph_edge
{
	double weight;
	edge_status stat_bfs;
	edge_status stat_dfs;
	int flow; //最大流算法中边的流量
	int capacity; //用于最大流算法中表示边的最大容量
	W data;
	graph_edge(W d = W(), double w = 1.0, int f = 0, int c = 0, edge_status s_bfs = undetermined, edge_status s_dfs = undetermined) :weight(w), stat_bfs(s_bfs), stat_dfs(s_dfs), data(d), flow(f), capacity(c) {};
	void init_bfs()
	{
		stat_bfs = undetermined;
	}
	void init_dfs()
	{
		stat_dfs = undetermined;
	}
	void init_max_flow()
	{
		flow = 0;
	}
};

template<typename T, typename W> class graph
{
	friend bool operator<(std::tuple<int, int, double> a, std::tuple<int, int, double> b);
	friend bool operator>(std::tuple<int, int, double> a, std::tuple<int, int, double> b);
	friend bool operator==(std::tuple<int, int, double> a, std::tuple<int, int, double> b);
private:
	//顶点集与邻接矩阵中的元素是对应的，即邻接矩阵中第i行或第i列的顶点也对应着顶点集中的第i个顶点
	std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> matrix; //邻接矩阵
	std::vector<graph_vertex<T>> vertex; //顶点集
	int number;
	graph_type type; //有向或无向
public:
	graph(graph_type t = undirected) :type(t) {};
	graph(std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> m, std::vector<graph_vertex<T>> v, graph_type t) :matrix(m), vertex(v), number(v.size()), type(t) {}; //复制指针地址的构造函数
	graph(const graph& G) //由于已经有了一个复制指针地址的构造函数，因此自定义一个copy operator，使得到的新graph中，edge的指针指向的是一个新的变量
	{
		std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> new_matrix(G.number, std::vector<std::shared_ptr<graph_edge<W>>>(G.number, NULL));
		for (int i = 0;i < G.number;i++)
		{
			for (int j = 0;j < G.number;j++)
			{
				if (G.matrix[i][j])
				{
					auto temp = *G.matrix[i][j];
					new_matrix[i][j] = std::make_shared<graph_edge<W>>(temp);
				}
			}
		}
		matrix = new_matrix;
		vertex = G.vertex;
		number = G.number;
		type = G.type;
	}
	graph& operator=(const graph& G) //自定义copy assignment operator得到的graph，edge的指针指向的是一个新的变量
	{
		std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> new_matrix(G.number, std::vector<std::shared_ptr<graph_edge<W>>>(G.number, NULL));
		for (int i = 0;i < G.number;i++)
		{
			for (int j = 0;j < G.number;j++)
			{
				if (G.matrix[i][j])
				{
					auto temp = *G.matrix[i][j];
					new_matrix[i][j] = std::make_shared<graph_edge<W>>(temp);
				}
			}
		}
		matrix = new_matrix;
		vertex = G.vertex;
		number = G.number;
		type = G.type;
		return *this;
	}
	int get_size() const
	{
		return number;
	}
	void insert_vertex(T v)
	{
		vertex.emplace_back(v);
		matrix.push_back(std::vector<std::shared_ptr<graph_edge<W>>>(number + 1, NULL));
		for (auto i = matrix.begin();i < matrix.end() - 1;i++)
		{
			(*i).push_back(NULL);
		}
		number++;
	}
	void delete_vertex(int i)
	{
		for (int m = 0;m < number;m++)
		{
			if (matrix[i][m])
			{
				vertex[m].indegree--;
			}
			if (matrix[m][i])
			{
				vertex[m].outdegree--;
			}
		}
		number--;
		matrix.erase(i);
		for (int j = 0;j < number;j++)
		{
			matrix[j].erase(i);
		}
	}
	void insert_edge(int i, int j, W d = W(), double w = 1.0, int c = 0, int f = 0) //代表从编号i到编号j的顶点之间的边
	{
		std::shared_ptr<graph_edge<W>> newedge(new graph_edge<W>(d, w, f, c));
		matrix[i][j] = newedge;
		if (type == undirected)
		{
			matrix[j][i] = newedge;
			vertex[j].outdegree++;
			vertex[i].indegree++;
		}
		vertex[i].outdegree++;
		vertex[j].indegree++;
	}
	void delete_edge(int i, int j)
	{
		matrix[i][j] = NULL;
		vertex[i].outdegree--;
		vertex[j].indegree--;
	}
	void clear_bfs_status()
	{
		for (int i = 0;i < number;i++)
		{
			vertex[i].init_bfs();
			for (int j = 0;j < number;j++)
			{
				if (matrix[i][j])
				{
					matrix[i][j]->init_bfs();
				}
			}
		}
	}
	void clear_dfs_status()
	{
		for (int i = 0;i < number;i++)
		{
			vertex[i].init_dfs();
			for (int j = 0;j < number;j++)
			{
				if (matrix[i][j])
				{
					matrix[i][j]->init_dfs();
				}
			}
		}
	}
	void clear_maxflow_status()
	{
		for (int i = 0;i < number;i++)
		{
			for (int j = 0;j < number;j++)
			{
				matrix[i][j]->init_max_flow();
			}
		}
	}
	void broadth_first_search() //全局的广度优先搜索算法
	{
		clear_bfs_status();
		int t = 1;
		for (int i = 0;i < number;i++)
		{
			if (vertex[i].stat_bfs == undiscovered) //代表顶点还没有被访问过，对当前顶点做bfs
			{
				bfs(i, t);
				t++;
			}
		}
	}
	int bfs(int i,int t)//局部连通域的bfs，参数i代表vertex中的第i个顶点，参数t代表第几个连通域
	{
		std::queue<int> q;
		q.push(i);
		vertex[i].discover_time_bfs = 0; //修改开始结点的访问时间为t
		vertex[i].stat_bfs = discovered;
		while (q.size()) //只要队列不为空就一直循环
		{
			int node = q.front();
			vertex[node].region_bfs = t;
			q.pop();
			for (int j = 0;j < number;j++)
			{
				if (matrix[node][j] && vertex[j].stat_bfs == undiscovered)
				{
					vertex[j].stat_bfs = discovered;
					vertex[j].parent_bfs = node;
					vertex[j].discover_time_bfs = vertex[node].discover_time_bfs + 1;
					matrix[node][j]->stat_bfs = tree_edge;
					q.push(j);
				}
				else if (matrix[node][j] && vertex[j].stat_bfs != undiscovered)
				{
					if (vertex[node].discover_time_bfs > vertex[j].discover_time_bfs&&vertex[node].region_bfs == vertex[j].region_bfs) //两顶点必须在同一连通域内才会有后向边
					{
						matrix[node][j]->stat_bfs = backward_edge;
					}
					else //对于其它情况，都可以直接标记为跨边
					{
						matrix[node][j]->stat_bfs = cross_edge;
					}
				}
			}
			vertex[node].stat_bfs = visited;
		}
		return t;
	}
	void depth_first_search() //全局的深度优先搜索算法
	{
		clear_dfs_status();
		int t = 0;
		for (int i = 0;i < number;i++)
		{
			if (vertex[i].stat_dfs == undiscovered) //代表顶点还没有被访问过，对当前顶点做dfs
			{
				t = dfs(i, ++t);
			}
		}
	}
	int dfs(int i,int t) //局部连通域的深度优先搜索算法，i代表开始搜索的根顶点编号，t代表对当前顶点开始搜索时所对应的时间标签
	{
		std::stack<int> c; //借助栈来存放dfs搜索过程中的结点
		vertex[i].discover_time_dfs = t; //修改根结点的状态
		vertex[i].stat_dfs = discovered;
		c.push(i); //先将根结点推入栈中
		while (c.size()) //如果栈不为空，就一直循环下去
		{
			int mark = c.top();
			int j = 0;
			for (;j < number;j++)
			{
				if (matrix[mark][j] && vertex[j].stat_dfs == undiscovered) //如果遇到一个没有被访问过的结点，就把它推入栈中，并直接开始下一轮的循环
				{
					t++;
					c.push(j);
					vertex[j].parent_dfs = mark;
					vertex[j].discover_time_dfs = t;
					matrix[mark][j]->stat_dfs = tree_edge;
					vertex[j].stat_dfs = discovered;
					break;
				}
				else if (matrix[mark][j] && vertex[j].stat_dfs != undiscovered&&matrix[mark][j]->stat_dfs != tree_edge)//遇到访问过的结点，根据结点的时间标记来修改对应边的状态
				{
					if (vertex[j].discover_time_dfs > vertex[mark].discover_time_dfs&&vertex[j].stat_dfs == visited) //前向边，此时结点j已经访问完毕（当前的t值大于j的finish_time_dfs），且mark比j发现的时间早
					{
						matrix[mark][j]->stat_dfs = forward_edge;
					}
					if (vertex[j].discover_time_dfs < vertex[mark].discover_time_dfs&&vertex[j].stat_dfs == discovered)//后向边，此时结点j仍在栈内，还未访问完毕（j的finish_time_dfs为-1）
					{
						matrix[mark][j]->stat_dfs = backward_edge;
					}
					if (vertex[j].discover_time_dfs < vertex[mark].discover_time_dfs&&vertex[j].stat_dfs == visited)//横向边，此时结点j已经访问完毕（当前的t值大于j的finish_time_dfs），且mark比j发现的时间晚
					{
						matrix[mark][j]->stat_dfs = cross_edge;
					}
				}
			}
			if (j == number) //代表这个结点的所有边都已经访问完毕，可以将其标记为已访问过，并从栈中移除
			{
				t++;
				vertex[mark].stat_dfs = visited;
				vertex[mark].finish_time_dfs = t;
				c.pop();
			}
		}
		return t; //返回这一局部连通域全部访问完成之后对应的时间标记
	}
	std::vector<int> topological_sort() //返回值是结点编号的拓扑排序，可用于强连通分量函数中
	{
		depth_first_search();
		std::vector<std::pair<int, int>> temp(number, std::make_pair(0, 0));
		for (int i = 0;i < number;i++)
		{
			temp[i] = std::make_pair(i, vertex[i].finish_time_dfs);
		}
		stable_sort(temp.begin(), temp.end(), [](std::pair<int, int> a, std::pair<int, int> b) {return a.second > b.second;});//按照结点dfs完成时间的逆序来排序
		std::vector<int> topological_sequence(number, 0);
		for (int i = 0;i < number;i++)
		{
			topological_sequence[i] = temp[i].first;
		}
		return topological_sequence;
	}
	graph transpose_graph(graph G) //求原图的转置图，只对有向图有效（即所有的边都反向）
	{
		if (G.type == undirected)
		{
			return G;
		}
		std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> transposed_matrix(number, std::vector<std::shared_ptr<graph_edge<W>>>(number, NULL));
		for (int i = 0;i < number;i++)
		{
			for (int j = 0;j < number;j++)
			{
				if (G.matrix[j][i])
				{
					transposed_matrix[i][j] = new graph_edge<W>(G.matrix[j][i]);
				}
			}
		}
		graph<T, W> transposed_graph(transposed_matrix, G.vertex, G.type);
		for (int i = 0;i < number;i++)
		{
			std::swap(transposed_graph.vertex[i].indegree, transposed_graph.vertex[i].outdegree);
		}
		return transposed_graph;
	}
	std::vector<std::vector<graph_vertex<T>>> strongly_connected_components()
	{
		this->clear_bfs_status();
		this->clear_dfs_status();
		auto transposed_graph = transpose_graph(*this);
		auto topo_seq = topological_sort(); //用于确认对转置矩阵做dfs的顺序
		int t = 0;
		int cnt = 0;
		for (int i = 0;i < number;i++)
		{
			if (transposed_graph.vertex[topo_seq[i]].stat_dfs == undiscovered) //按照原矩阵拓扑排序的顺序对转置矩阵进行dfs
			{
				t = transposed_graph.dfs(topo_seq[i], ++t);
				cnt++;
			}
		}
		std::vector<std::vector<graph_vertex<T>>> scc(cnt, std::vector<graph_vertex<T>>(0));
		int memo_beg = 1;
		int memo_end = 0;
		for (int i = 0;i < cnt;i++) //按照转置矩阵的dfs结点访问顺序，对结点进行分组
		{
			for (int n = 0;n < number;n++)
			{
				if (transposed_graph.vertex[n].discover_time_dfs == memo_beg)
				{
					memo_end = transposed_graph.vertex[n].finish_time_dfs;
					for (int j = 0;j < number;j++)
					{
						if (transposed_graph.vertex[j].discover_time_dfs >= memo_beg&&transposed_graph.vertex[j].finish_time_dfs <= memo_end)
						{
							scc[i].push_back(transposed_graph.vertex[j]);
						}
					}
					memo_beg = memo_end + 1;
					break;
				}
			}
		}
		return scc;
	}
	std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> MST_Kruskal() //使用Kruskal算法生成的最小生成树
	{
		std::vector<std::tuple<std::shared_ptr<graph_edge<W>>, int, int>> edges;//存储边及它在矩阵中所对应的位置
		for (int i = 0;i < number;i++) //将所有的边加入到edges变量中
		{
			for (int j = i;j < number;j++)
			{
				if (matrix[i][j])
				{
					edges.push_back(std::make_tuple(matrix[i][j], i, j));
				}
			}
		}
		std::stable_sort(edges.begin(), edges.end(), [](std::tuple<std::shared_ptr<graph_edge<W>>, int, int> a, std::tuple<std::shared_ptr<graph_edge<W>>, int, int> b) {return std::get<0>(a)->weight < std::get<0>(b)->weight;}); //按照权重对边进行排序，此处也可以调用之前写的快排，堆排等算法，这样会使用在上面定义的边比较运算符
		std::vector<std::shared_ptr<set_forest_node<graph_vertex<T>>>> vertex_set; //将每个顶点都看作一个独立的集合，在生成MST的时候对顶点所在集合做合并
		for (int i = 0;i < number;i++) 
		{
			vertex_set.push_back(std::shared_ptr<set_forest_node<graph_vertex<T>>>(new set_forest_node<graph_vertex<T>>(vertex[i])));
			vertex_set[i]->node_fix();
		}
		std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> MST(number, std::vector<std::shared_ptr<graph_edge<W>>>(number, NULL)); //保存最后MST中的边
		for (int i = 0;i < edges.size();i++)
		{
			if (find_set(vertex_set[std::get<1>(edges[i])]) != find_set(vertex_set[std::get<2>(edges[i])])) //如果两个顶点不属于同一个集合，那么将此边加入到最小生成树中不会成环
			{
				MST[std::get<1>(edges[i])][std::get<2>(edges[i])] = std::get<0>(edges[i]);
				MST[std::get<2>(edges[i])][std::get<1>(edges[i])] = std::get<0>(edges[i]);
				union_set(vertex_set[std::get<1>(edges[i])], vertex_set[std::get<2>(edges[i])]); //将加入MST的边所对应的两个顶点合并为同一个集合
			}
		}
		return MST;
	}
	std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> MST_Prim() //prim算法生成的最小生成树，在任意一个时刻，所有已经选择的顶点
	{
		Fibonacci_heap<std::tuple<int, int, double>> vertex_heap; //储存顶点及其信息的一个斐波那契堆，tuple中的第一个元素代表顶点编号，第二个元素代表父结点的编号，第三个代表当前顶点所连接的权重最小的边（不包括已经取出的顶点）
		std::vector<std::shared_ptr<Fibonacci_heap_node<std::tuple<int, int, double>>>> node;
		std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> MST(number, std::vector<std::shared_ptr<graph_edge<W>>>(number, NULL)); //保存最后MST中的边
		for (int i = 0;i < number;i++) //将node保存起来，以便decrease_key的时候使用，同时把元素插入到fibonacci堆中
		{
			node.push_back(std::shared_ptr<Fibonacci_heap_node<std::tuple<int, int, double>>>(new Fibonacci_heap_node<std::tuple<int, int, double>>(NULL, NULL, NULL, NULL, 0, std::make_tuple(i, -1, LDBL_MAX), false, true)));
			vertex_heap.insert_node(node[i]);
		}
		vertex_heap.decrease_key(node[0], std::make_tuple(0, -1, 0.0));
		while (vertex_heap.get_size())
		{
			auto temp = vertex_heap.extract_min()->key;
			if (std::get<1>(temp) != -1) //将对应边插入到MST矩阵中
			{
				MST[std::get<1>(temp)][std::get<0>(temp)] = MST[std::get<0>(temp)][std::get<1>(temp)] = matrix[std::get<0>(temp)][std::get<1>(temp)];
			}
			for (auto i = 0;i < number;i++)
			{
				if (matrix[std::get<0>(temp)][i] && node[i]->inside) //查找到边，并且这条边所对应的另一个顶点还在堆内
				{
					if (std::get<2>(node[i]->key)>matrix[std::get<0>(temp)][i]->weight)//如果堆内这个顶点当前记录的最小权重小于新发现的权重，就修改它
					{
						vertex_heap.decrease_key(node[i], std::make_tuple(i, std::get<0>(temp), matrix[std::get<0>(temp)][i]->weight));
					}
				}
			}
		}
		return MST;
	}
	std::pair<std::vector<double>, std::vector<int>> Bellman_Ford(int root) //第一个vector保存权重，第二个保存父结点
	{
		std::vector<double> distance(number, LDBL_MAX); //用于保存结点的距离，距离为0的那个点代表开始搜索的点
		std::vector<int> parent(number, -1);
		distance[root] = 0;
		int count = 0; //用count两个变量来决定何时跳出循环，如果不存在权重为负的环路，那么最短路径的长度最多是V-1，因此当count=V-1的时候再做最后一次循环，如果最短路径仍被修改，那就说明出现负权重的环路
		bool stat = false; //用于确定循环过程中是否对最小路径做出修改
		while (count < number)
		{
			stat = true; //false代表整个循环过程中最小路径被修改，true代表循环的时候未被修改过
			for (int i = 0;i < number;i++)
			{
				for (int j = 0;j < number;j++)
				{
					if (matrix[i][j] && distance[i] != LDBL_MAX && distance[i] + matrix[i][j]->weight < distance[j])
					{
						stat = false;
						distance[j] = distance[i] + matrix[i][j]->weight;
						parent[j] = i;
					}
				}
			}
			count++;
		}
		if (!stat) //如果不存在负权重的环路，那么while循环的最后一步不会做任何操作，这里的if不会被执行。如果执行，说明存在负权重环路，则把环路输出到命令行中
		{
			std::cout << "The graph contains negative weight circuit: ";
			for (int i = 0;i < number;i++)
			{
				for (int j = 0;j < number;j++)
				{
					if (matrix[i][j] && distance[i] != LDBL_MAX && distance[i] + matrix[i][j]->weight < distance[j])
					{
						std::cout << i << "->" << j << " ";
					}
				}
			}
			std::cout << std::endl;
		}
		return std::make_pair(distance, parent);
	}
	std::pair<std::vector<double>, std::vector<int>> DAG_shortest_path(int i) //对于DAG图求最小距离，先对结点做拓扑排序，然后只用一遍循环即可完成所有结点距离的确定
	{
		auto seq = topological_sort();
		std::vector<double> distance(number, LDBL_MAX);
		distance[seq[i]] = 0;
		for (auto i : seq)
		{
			for (auto j = 0;j < number;j++)
			{
				if (matrix[i][j] && distance[i] + matrix[i][j]->weight < distance[j])
				{
					distance[j] = distance[i] + matrix[i][j]->weight;
				}
			}
		}
		return std::make_pair(distance, seq);
	}
	std::pair<std::vector<double>, std::vector<int>> Dijkstra(int root) //用dijkstra算法求最小距离，要求所有边的权重都为非负值。与prim算法的思路类似，用斐波那契堆来保存结点
	{
		Fibonacci_heap<std::tuple<int, int, double>> vertex_heap; //储存顶点及其信息的一个斐波那契堆，tuple中的第一个元素代表顶点编号，第二个元素代表父结点的编号，第三个代表只使用已经取出的顶点所能组成的最短路径的权重
		std::vector<std::shared_ptr<Fibonacci_heap_node<std::tuple<int, int, double>>>> node;
		for (int i = 0;i < number;i++) //将node保存起来，以便decrease_key的时候使用，同时把元素插入到fibonacci堆中
		{
			node.push_back(std::shared_ptr<Fibonacci_heap_node<std::tuple<int, int, double>>>(new Fibonacci_heap_node<std::tuple<int, int, double>>(NULL, NULL, NULL, NULL, 0, std::make_tuple(i, -1, LDBL_MAX), false, true)));
			vertex_heap.insert_node(node[i]);
		}
		vertex_heap.decrease_key(node[root], std::make_tuple(root, -1, 0.0)); //将根结点的距离设置为0
		while (vertex_heap.get_size())
		{
			auto temp = vertex_heap.extract_min()->key;
			for (auto i = 0;i < number;i++)
			{
				if (matrix[std::get<0>(temp)][i] && node[i]->inside && std::get<2>(temp) + matrix[std::get<0>(temp)][i]->weight < std::get<2>(node[i]->key)) //查找到边
				{
					vertex_heap.decrease_key(node[i], std::make_tuple(i, std::get<0>(temp), std::get<2>(temp) + matrix[std::get<0>(temp)][i]->weight)); //发现更短的路径，做出修改
				}
			}
		}
		std::vector<double> distance(number, 0); //用于保存结点的距离，距离为0的那个点代表开始搜索的点
		std::vector<int> parent(number, -1);
		for (auto i = 0;i < number;i++)
		{
			distance[i] = std::get<2>(node[i]->key);
			parent[i] = std::get<1>(node[i]->key);
		}
		return std::make_pair(distance, parent);
	}
	std::pair<std::vector<std::vector<double>>,std::vector<std::vector<int>>> all_pairs_shortest_paths() //从最小路径长度为0开始，一直到不小于v-1
	{
		std::vector<std::vector<double>> distance(number, std::vector<double>(number,LDBL_MAX)); //i,j位置代表从点i到点j的最短权重路径
		std::vector<std::vector<int>> parent(number, std::vector<int>(number, -1)); //i,j位置代表从点i到点j的最短权重路径上，j的前驱结点
		for (int i = 0;i < number;i++) //先给distance矩阵初始化
		{
			for (int j = 0;j < number;j++)
			{
				if (i == j)
				{
					distance[i][j] = 0;
				}
				else if (matrix[i][j])
				{
					distance[i][j] = matrix[i][j]->weight;
					parent[i][j] = i;
				}
			}
		}
		for (int i = 0;i < ceil(log(number));i++) //每次循环开始时i等于几，就代表最小路径只经过2^i个中间点
		{
			auto temp_distance = distance; //需要用中间变量来保存计算过程中得到的值
			auto temp_parent = parent;
			for (int x = 0;x < number;x++)
			{
				for (int y = 0;y < number;y++)
				{
					for (int k = 0;k < number;k++)
					{
						if (temp_distance[x][y]>distance[x][k] + distance[k][y])
						{
							temp_distance[x][y] = distance[x][k] + distance[k][y];
							temp_parent[x][y] = parent[k][y];
						}
					}
				}
			}
			distance = temp_distance;
			parent = temp_parent;
		}
		for (int x = 0;x < number;x++) //检测负权重环路
		{
			for (int y = 0;y < number;y++)
			{
				for (int k = 0;k < number;k++)
				{
					if (distance[x][y]>distance[x][k] + distance[k][y])
					{
						std::cout << "The graph contains negative weight circult." << std::endl;
						return std::make_pair(distance, parent);
					}
				}
			}
		}
		return std::make_pair(distance, parent);
	}
	std::pair<std::vector<std::vector<double>>, std::vector<std::vector<int>>> Floyd_Warshall() //最小路径经过编号为0-i的顶点，一直循环下去，直至i=v
	{
		std::vector<std::vector<double>> distance(number, std::vector<double>(number, LDBL_MAX)); //i,j位置代表从点i到点j的最短权重路径
		std::vector<std::vector<int>> parent(number, std::vector<int>(number, -1)); //i,j位置代表从点i到点j的最短权重路径上，j的前驱结点
		for (int i = 0;i < number;i++) //先给distance矩阵初始化
		{
			for (int j = 0;j < number;j++)
			{
				if (i == j)
				{
					distance[i][j] = 0;
				}
				else if (matrix[i][j])
				{
					distance[i][j] = matrix[i][j]->weight;
					parent[i][j] = i;
				}
			}
		}
		for (int i = 0;i < number;i++) //每次循环开始时i等于几，就代表最小路径只经过编号为0-i的点
		{
			for (int x = 0;x < number;x++)
			{
				for (int y = 0;y < number;y++) //循环过程中，已经被修改的值不会影响计算得到的值，因此可以不用临时变量来保存结果
				{
					if (distance[x][y]>distance[x][i] + distance[i][y])
					{
						distance[x][y] = distance[x][i] + distance[i][y];
						parent[x][y] = parent[i][y];
					}
				}
			}
		}
		for (int x = 0;x < number;x++) //检测是否有负权重的环路
		{
			for (int y = 0;y < number;y++)
			{
				for (int k = 0;k < number;k++)
				{
					if (distance[x][y]>distance[x][k] + distance[k][y])
					{
						std::cout << "The graph contains negative weight circult." << std::endl;
						return std::make_pair(distance, parent);
					}
				}
			}
		}
		return std::make_pair(distance, parent);
	}
	std::pair<std::vector<std::vector<double>>, std::vector<std::vector<int>>> Johnson() //重新赋予权重之后不改变最短路径，利用此性质可以先修改所有边的权重为非负，然后对修改权重后的图中每个顶点都调用一次Dijkstra算法即可，这样使得时间复杂度低于V^3
	{
		graph newgraph = *this;
		newgraph.insert_vertex(T()); //为原图添加一个新的顶点，以及连接各个原顶点的权重为0的边
		for (int i = 0;i < number;i++)
		{
			newgraph.insert_edge(number, i);
		}
		std::vector<double> dis = newgraph.Bellman_Ford(number).first; //从新顶点到其它顶点的最短路径，用于改变边的权重，使权重非负
		newgraph = *this;
		for (int i = 0;i < number;i++)
		{
			for (int j = 0;j < number;j++)
			{
				if (newgraph.matrix[i][j])
				{
					newgraph.matrix[i][j]->weight += (dis[i] - dis[j]); //根据三角不等式，d[i]+w[i][j]>d[j]，根据此性质对每条边的权重做修改，使其变为非负的权重
				}
			}
		}
		std::vector<std::vector<double>> distance;
		std::vector<std::vector<int>> parent;
		for (int i = 0;i < number;i++)
		{
			auto temp = newgraph.Dijkstra(i); //由于所有边的权重非负，就可以直接对每个顶点都调用一次Dijkstra算法
			distance.push_back(temp.first);
			parent.push_back(temp.second);
		}
		for (int i = 0;i < number;i++)
		{
			for (int j = 0;j < number;j++)
			{
				distance[i][j] -= (dis[i] - dis[j]); //之前计算出的是修改权重之后的最短路径，还要根据w'[p]=w[p]+h[i]-h[j]对最短路径进行还原
			}
		}
		return std::make_pair(distance, parent);
	}
	void Ford_Fulkerson() //用于最大流搜索的Ford_fulkerson算法，运行的时候要求第一个结点是源节点s，最后一个结点是汇点t
	{
		graph residual_network = *this;//创建一个graph变量作为残存网络，每一步操作之后都对残存网络做相应的修改，这样就避免了每次都做复制操作。用capacity变量表示这条边可以推送的流量
		while (true)
		{
			residual_network.clear_bfs_status();
			residual_network.bfs(0, 0);
			if (residual_network.vertex[number-1].parent_bfs < 0) //代表不存在s到t的增广路径，即已经获得一个最大流，可以直接退出循环
			{
				break;
			}
			int max_flow = INT_MAX;
			int point = number - 1;
			while (residual_network.vertex[point].parent_bfs >= 0) //由于是从源节点s开始搜索的，因此只要做bfs时汇点t的父结点不为空，则代表存在一条s到t的增广路径，先寻找这条路径上的容量最小值
			{
				if (residual_network.matrix[residual_network.vertex[point].parent_bfs][point]->capacity < max_flow)
				{
					max_flow = residual_network.matrix[residual_network.vertex[point].parent_bfs][point]->capacity;
				}
				point = residual_network.vertex[point].parent_bfs;
			}
			point = number - 1;
			while (residual_network.vertex[point].parent_bfs >= 0) //找到增广路径之后，对原图中这条路径上的流量进行修改，并修改残存网络
			{
				if (residual_network.matrix[residual_network.vertex[point].parent_bfs][point]) //对原图中路径上的流量进行修改
				{
					matrix[residual_network.vertex[point].parent_bfs][point]->flow += max_flow;
				}
				else
				{
					matrix[point][residual_network.vertex[point].parent_bfs]->flow -= max_flow;
				}

				residual_network.matrix[residual_network.vertex[point].parent_bfs][point]->capacity -= max_flow;
				if (residual_network.matrix[residual_network.vertex[point].parent_bfs][point]->capacity == 0) //如果某条边的容量已经满，需要在残存网络中删除掉这条边
				{
					residual_network.delete_edge(residual_network.vertex[point].parent_bfs, point);
				}
				if (residual_network.matrix[point][residual_network.vertex[point].parent_bfs]) //如果残存网络中已经存在反向边，则直接修改该边的流量即可
				{
					residual_network.matrix[point][residual_network.vertex[point].parent_bfs]->capacity += max_flow;
				}
				else //如果不存在该反向边，则需要添加一条新边，并修改流量
				{
					residual_network.insert_edge(point, residual_network.vertex[point].parent_bfs);
					residual_network.matrix[point][residual_network.vertex[point].parent_bfs]->capacity += max_flow;
				}
				point = residual_network.vertex[point].parent_bfs;
			}
		}
	}
	void maxflow_push_relabel() //用于最大流搜索的推送-重贴标签算法，运行的时候要求第一个结点是源节点s，最后一个结点是汇点t
	{
		graph residual_network = *this; //残存网络，用capacity变量表示这条边可以推送的最大流量
		std::vector<int> height(number, 0); //保存每个结点的高度，并将源节点s的高度设为V
		height[0] = number;
		std::queue<int> overflow_vertex; //用于保存有超额流结点的队列
		std::vector<int> overflow(number, 0); //保存每个结点的多余流量
		std::vector<std::list<int>> high(number - 1, std::list<int>()); //保存残存网络中每个结点的相邻结点中，高度小于这个结点者。如果list为空，那么就说明需要做relabel操作，否则需要做push操作
		for (int i = 0;i < number;i++) //初始化操作
		{
			if (matrix[0][i])
			{
				overflow[i] = matrix[0][i]->capacity;
				matrix[0][i]->flow = matrix[0][i]->capacity;
				residual_network.delete_edge(0, i);
				residual_network.insert_edge(i, 0);
				residual_network.matrix[i][0]->capacity = matrix[0][i]->flow;
				if (i != number - 1)
				{
					overflow_vertex.push(i);
				}
			}
		}
		while (overflow_vertex.size()) //只要队列里面有结点，就将循环进行下去
		{
			int node = overflow_vertex.front(); //取出队列中的元素
			auto ptr = high[node].begin();
			if (high[node].size()) //如果结点对应的list不为空，就做push操作
			{
				while (ptr!=high[node].end() && overflow[node]) //如果结点仍有多余流量，并且有可以推送的结点，就一直循环下去
				{
					int temp = *ptr;
					if (residual_network.matrix[node][temp])
					{
						int push_flow = residual_network.matrix[node][temp]->capacity < overflow[node] ? residual_network.matrix[node][temp]->capacity : overflow[node];
						overflow[node] -= push_flow;
						if (temp != 0 && temp != number - 1) //如果推送的目标结点不是源节点和汇点，需要做一些修改
						{
							overflow[temp] += push_flow;
							overflow_vertex.push(temp);
						}
						if (matrix[node][temp]) //对原图中路径上的流量进行修改
						{
							matrix[node][temp]->flow += push_flow;
						}
						else
						{
							matrix[temp][node]->flow -= push_flow;
						}

						residual_network.matrix[node][temp]->capacity -= push_flow;
						if (residual_network.matrix[node][temp]->capacity == 0) //如果某条边的容量已经满，需要在残存网络中删除掉这条边
						{
							residual_network.delete_edge(node, temp);
						}
						if (residual_network.matrix[temp][node]) //如果残存网络中已经存在反向边，则直接修改该边的流量即可
						{
							residual_network.matrix[temp][node]->capacity += push_flow;
						}
						else //如果不存在该反向边，则需要添加一条新边，并修改流量
						{
							residual_network.insert_edge(temp, node);
							residual_network.matrix[temp][node]->capacity += push_flow;
						}
						ptr++;
					}
					else 
					{
						auto temp = ptr;
						ptr++;
						high[node].erase(temp);
					}
				}
				if (!overflow[node]) //循环结束之后，如果没有多余的流量，就从队列中删除这个结点
				{
					overflow_vertex.pop();
				}
			}
			else //做relabel操作
			{
				std::list<int> height_node;
				int min_height = INT_MAX;
				for (int i = 0;i < number;i++) //寻找残存网络中可以到达的高度最低的结点，以及这个高度值
				{
					if (residual_network.matrix[node][i])
					{
						if (height[i] < min_height)
						{
							height_node.clear();
							min_height = height[i];
							height_node.push_back(i);
						}
						if (height[i] == min_height)
						{
							height_node.push_back(i);
						}
					}
				}
				height[node] = min_height + 1;
				high[node] = height_node;
				for (auto i : high[node])
				{
					if (i != number - 1)
					{
						auto j = high[i].begin();
						while (j != high[i].end())
						{
							if (*j == node)
							{
								auto temp = j;
								j++;
								high[i].erase(temp);
							}
							else
							{
								j++;
							}
						}
					}
				}
			}
		}
	}
	void maxflow_relabel_to_front()
	{
		std::vector<int> height(number, 0); //保存每个结点的高度
		height[0] = number;
		std::vector<int> overflow(number, 0); //保存每个结点的多余流量
		for (int i = 0;i < number;i++)
		{
			if (matrix[0][i])
			{
				overflow[i] = matrix[0][i]->capacity;
				matrix[0][i]->flow = matrix[0][i]->capacity;
			}
		}
		std::vector<std::list<int>> neighbor(number - 1, std::list<int>()); //保存每个结点的相邻结点
		std::list<int> sequence; //代表运行过程中结点进行访问的顺序，在程序运行过程中可能会被调整
		for (int i = 1;i < number - 1;i++) 
		{
			for (int j = 0;j < number;j++)
			{
				if (matrix[i][j] || matrix[j][i])
				{
					neighbor[i].push_back(j);
				}
			}
			sequence.push_back(i);
		}
		std::vector<std::list<int>::iterator> current(number, sequence.begin()); //每一个位置都对应着neighbor中同样位置上的list
		for (auto i = 1;i < number - 1;i++)
		{
			current[i] = neighbor[i].begin();
		}
		auto label = sequence.begin(); //作为sequence中目前指向位置的标记
		while (label != sequence.end()) //当这个标记指向neighbor的最末端时，则获得全图的一个最大流
		{
			int old_height = height[*label];
			while (overflow[*label]) //如果某结点有多余的流量，则对该结点做处理，直到多余流量消失
			{
				auto ptr = current[*label]; 
				if (ptr == neighbor[*label].end())
				{
					int min_height = INT_MAX;
					for (auto i : neighbor[*label]) //寻找残存网络中可以到达的高度最低的结点，以及这个高度值
					{
						if (matrix[i][*label]) //如果i对应的边是一条入边
						{
							if (matrix[i][*label]->flow&&height[i] < min_height) //此时，入边上必须有流量才对其进行考虑，否则无法推送
							{
								min_height = height[i];
							}
						}
						else if(matrix[*label][i]) //如果i对应一条出边
						{
							if (matrix[*label][i]->flow<matrix[*label][i]->capacity&&height[i] < min_height) //如果出边仍有多余容量可以推送，才对其进行考虑
							{
								min_height = height[i];
							}
						}
					}
					height[*label] = min_height + 1; //修改结点对应的高度
					current[*label] = neighbor[*label].begin(); //修改current
				}
				else if (height[*label] == height[*ptr] + 1) //寻找结点做推送操作
				{
					int flow = overflow[*label];
					if (matrix[*ptr][*label]) //如果ptr对应的边是一条入边
					{
						if (matrix[*ptr][*label]->flow < flow)
						{
							flow = matrix[*ptr][*label]->flow;
						}
						matrix[*ptr][*label]->flow -= flow;
						overflow[*label] -= flow;
						if (*ptr != 0 && *ptr != number - 1)
						{
							overflow[*ptr] += flow;
						}
					}
					else  //如果ptr对应一条出边
					{
						if (matrix[*label][*ptr]->capacity - matrix[*label][*ptr]->flow < flow)
						{
							flow = matrix[*label][*ptr]->capacity - matrix[*label][*ptr]->flow;
						}
						matrix[*label][*ptr]->flow += flow;
						overflow[*label] -= flow;
						if (*ptr != 0 && *ptr != number - 1)
						{
							overflow[*ptr] += flow;
						}
					}
					if (overflow[*label] > 0) //如果推送完之后仍有多余的流量，则将current中存储的指针下移
					{
						current[*label]++;
					}
				}
				else
				{
					current[*label]++;
				}
			}
			if (height[*label] > old_height) //如果高度发生变化，就把这个结点移到队列的最前端
			{
				auto temp = *label;
				sequence.erase(label);
				sequence.push_front(temp);
				label = sequence.begin();
			}
			label++;
		}
	}
	template<typename U> void traverse_vertex(U op_vertex)
	{
		for (auto i : vertex)
		{
			op_vertex(i);
		}
	}
	template<typename U> void traverse_edge(U op_edge)
	{
		for (int i = 0;i < number;i++)
		{
			for (int j = 0;j < number;j++)
			{
				if (matrix[i][j])
				{
					op_edge(i, j, matrix[i][j]);
				}
			}
		}
	}
};

/*
以clrs22.3.2的练习题为例测试bfs/dfs/scc三个算法：
template<typename T> struct print_vertex_bfs
{
	void operator()(graph_vertex<T> v)
	{
		std::cout << v.key << " Start time: " << v.discover_time_bfs << '\t' << "Region:" << v.region_bfs << '\t' << "Parent: " << v.parent_bfs << '\t' << std::endl;
	}
};
template<typename T> struct print_vertex_dfs
{
	void operator()(graph_vertex<T> v)
	{
		std::cout << v.key << " Start time: " << v.discover_time_dfs << '\t' << " Finish time: " << v.finish_time_dfs << '\t' << "Parent: " << v.parent_dfs << '\t' << std::endl;
	}
};
template<typename U> struct print_edge_bfs
{
	void operator()(int i, int j, std::shared_ptr<graph_edge<U>> e)
	{
		std::cout << "Edge from vertex " << i << " to " << j << ":" << e->stat_bfs << std::endl;
	}
};
template<typename U> struct print_edge_dfs
{
	void operator()(int i, int j, std::shared_ptr<graph_edge<U>> e)
	{
		std::cout << "Edge from vertex " << i << " to " << j << ":" << e->stat_dfs << std::endl;
	}
};

int main()
{
	graph<char, int> test(directed);
	print_edge_bfs<int> e_bfs;
	print_edge_dfs<int> e_dfs;
	print_vertex_bfs<char> v_bfs;
	print_vertex_dfs<char> v_dfs;
	test.insert_vertex('v');
	test.insert_vertex('s');
	test.insert_vertex('w');
	test.insert_vertex('q');
	test.insert_vertex('t');
	test.insert_vertex('x');
	test.insert_vertex('z');
	test.insert_vertex('y');
	test.insert_vertex('r');
	test.insert_vertex('u');
	test.insert_edge(1, 0, 0);
	test.insert_edge(0, 2, 0);
	test.insert_edge(2, 1, 0);
	test.insert_edge(3, 1, 0);
	test.insert_edge(3, 2, 0);
	test.insert_edge(3, 4, 0);
	test.insert_edge(4, 7, 0);
	test.insert_edge(4, 5, 0);
	test.insert_edge(5, 6, 0);
	test.insert_edge(6, 5, 0);
	test.insert_edge(7, 3, 0);
	test.insert_edge(9, 7, 0);
	test.insert_edge(8, 7, 0);
	test.insert_edge(8, 9, 0);
	test.broadth_first_search();
	std::cout << "BFS:" << std::endl;
	test.traverse_edge(e_bfs);
	test.traverse_vertex(v_bfs);
	test.depth_first_search();
	std::cout << "DFS:" << std::endl;
	test.traverse_edge(e_dfs);
	test.traverse_vertex(v_dfs);
	auto p = test.strongly_connected_components();
	std::cout << "Strongly connect components:" << std::endl;
	for (auto i : p)
	{
		for (auto j : i)
		{
			std::cout << j.key << '\t';
		}
		std::cout << std::endl;
	}
	return 0;
}
*/

/*
测试最小生成树的代码（与clrs23.2的示例图相同）
int main()
{
	graph<char, int> test(undirected);
	test.insert_vertex('a');
	test.insert_vertex('b');
	test.insert_vertex('c');
	test.insert_vertex('d');
	test.insert_vertex('e');
	test.insert_vertex('f');
	test.insert_vertex('g');
	test.insert_vertex('h');
	test.insert_vertex('i');
	test.insert_edge(0, 1, int(), 4);
	test.insert_edge(1, 2, int(), 8);
	test.insert_edge(2, 3, int(), 7);
	test.insert_edge(3,4, int(), 9);
	test.insert_edge(4,5, int(), 10);
	test.insert_edge(3,5, int(), 14);
	test.insert_edge(5,6, int(), 2);
	test.insert_edge(6,7, int(), 1);
	test.insert_edge(7,8, int(), 7);
	test.insert_edge(6,8, int(), 6);
	test.insert_edge(2,8, int(), 2);
	test.insert_edge(2,5, int(), 4);
	test.insert_edge(1,7, int(), 11);
	test.insert_edge(0, 7, int(), 8);
	auto result = test.MST_Kruskal();
	auto result2 = test.MST_Prim();
	std::cout << "MST Kruskal algorithm:" << std::endl;
	for (int i = 0;i < result.size();i++)
	{
		for (int j = 0;j < result.size();j++)
		{
			if (result[i][j])
			{
				std::cout << 1 << " ";
			}
			else
			{
				std::cout << 0 << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "MST Prim algorithm:" << std::endl;
	for (int i = 0;i < result.size();i++)
	{
		for (int j = 0;j < result.size();j++)
		{
			if (result2[i][j])
			{
				std::cout << 1 << " ";
			}
			else
			{
				std::cout << 0 << " ";
			}
		}
		std::cout << std::endl;
	}
	return 0;
}
*/

/*
测试单源最短路径的代码（clrs图24-6）：
int main()
{
	graph<char, int> test(directed);
	test.insert_vertex('a');
	test.insert_vertex('b');
	test.insert_vertex('c');
	test.insert_vertex('d');
	test.insert_vertex('e');
	test.insert_edge(0,1,int(),10);
	test.insert_edge(0,4, int(), 5);
	test.insert_edge(1,4, int(), 2);
	test.insert_edge(1,2, int(), 1);
	test.insert_edge(1,4, int(), 2);
	test.insert_edge(2,3, int(), 4);
	test.insert_edge(3,2,int(), 6);
	test.insert_edge(3,0, int(), 7);
	test.insert_edge(4,1, int(), 3);
	test.insert_edge(4,2, int(), 9);
	test.insert_edge(4,3, int(), 2);
	auto j = test.Dijkstra(0).first;
	auto i = test.Bellman_Ford(0).first;
	for (auto x : i)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
	for (auto x : j)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
	auto m = test.Bellman_Ford(0).second;
	auto n = test.Dijkstra(0).second;
	for (auto x : m)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
	for (auto x : n)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
	return 0;
}
*/

/*
DAG图的测试代码（以clrs图24-5为例）：
int main()
{
	graph<char, int> test(directed);
	test.insert_vertex('a');
	test.insert_vertex('b');
	test.insert_vertex('c');
	test.insert_vertex('d');
	test.insert_vertex('e');
	test.insert_vertex('f');
	test.insert_edge(0,1,int(),5);
	test.insert_edge(0,2, int(), 3);
	test.insert_edge(1,2, int(), 2);
	test.insert_edge(1,3, int(), 6);
	test.insert_edge(2,3, int(), 7);
	test.insert_edge(2,4, int(), 4);
	test.insert_edge(2,5, int(), 2);
	test.insert_edge(3,4, int(), -1);
	test.insert_edge(3,5, int(), 1);
	test.insert_edge(4,5, int(), -2);
	auto dis = test.DAG_shortest_path(1).first;
	auto sequence = test.DAG_shortest_path(1).second;
	for (auto i : dis)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (auto i : sequence)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
	return 0;
}
*/

/*
测试所有结点对最短路径的源代码（以clrs图25-6为例）：
int main()
{
	graph<char, int> test(directed);
	test.insert_vertex('a');
	test.insert_vertex('b');
	test.insert_vertex('c');
	test.insert_vertex('d');
	test.insert_vertex('e');
	test.insert_edge(0, 1, int(), 3);
	test.insert_edge(0,2, int(), 8);
	test.insert_edge(0,4, int(), -4);
	test.insert_edge(1,3, int(), 1);
	test.insert_edge(1,4, int(), 7);
	test.insert_edge(2,1, int(), 4);
	test.insert_edge(3,2, int(), -5);
	test.insert_edge(3,0, int(), 2);
	test.insert_edge(4,3, int(), 6);
	auto result = test.all_pairs_shortest_paths(); 
	auto distance = result.first;
	auto parent = result.second;
	std::cout << "all-pair shortest path by matrix multiplication:" << std::endl;
	for (auto i : distance)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (auto i : parent)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	result = test.Floyd_Warshall();
	distance = result.first;
	parent = result.second;
	std::cout << "all-pair shortest path by Floyd-warshall algorithm:" << std::endl;
	for (auto i : distance)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (auto i : parent)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	result = test.Johnson();
	distance = result.first;
	parent = result.second;
	std::cout << "all-pair shortest path by Johnson algorithm:" << std::endl;
	for (auto i : distance)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (auto i : parent)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return 0;
}
*/

/*
测试最大流算法的程序（以clrs图26-6为例）：
template<typename U> struct print_edge_maximum_flow
{
	void operator()(int i, int j, std::shared_ptr<graph_edge<U>> e)
	{
		std::cout << "Edge from " << i << " to " << j << ": " << e->flow << "/" << e->capacity << std::endl;
	}
};

int main()
{
	print_edge_maximum_flow<int> print;
	graph<std::string, int> test(directed);
	test.insert_vertex("s");
	test.insert_vertex("v1");
	test.insert_vertex("v2");
	test.insert_vertex("v3");
	test.insert_vertex("v4");
	test.insert_vertex("t");
	test.insert_edge(0,1, int(),0,16 );
	test.insert_edge(0,2 ,int(), 0,13 );
	test.insert_edge(1,3 ,int(), 0,12 );
	test.insert_edge(2,1 ,int(), 0,4 );
	test.insert_edge(2,4 ,int(), 0,14 );
	test.insert_edge(3,2 ,int(), 0,9 );
	test.insert_edge(3,5 ,int(), 0,20 );
	test.insert_edge(4,3 ,int(), 0,7 );
	test.insert_edge(4,5 ,int(), 0,4 );
	test.Ford_Fulkerson();
	test.maxflow_push_relabel();
	test.maxflow_relabel_to_front();
	test.traverse_edge(print);
	return 0;
}
*/