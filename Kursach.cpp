#include <iostream>
#include <set>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;
ofstream fout("answer.txt");

// Вершина (хранит только свое имя)
struct Node {
    int name;
};

// Дуга (хранит указатели на исходную и конечную вершины)
struct Edge {
    Node* startNode;
    Node* endNode;

    // Конструктор с передаваемыми параметрами (две связанные вершины)
    Edge(Node* stNode, Node* enNode) {
        startNode = stNode;
        endNode = enNode;
    }

    // Оператор сравнения, необходим для добавления в множество
    bool operator < (const Edge& otherEdge) const {
        // Если оба указателя совпадают, то не добавляем такую дугу
        if (this->startNode == otherEdge.startNode && this->endNode == otherEdge.endNode
            || this->startNode == otherEdge.endNode && this->endNode == otherEdge.startNode)
            return false;

        // Если один из указателей совпал, сравниваем по второму
        if (this->startNode == otherEdge.startNode)
            return this->endNode < otherEdge.endNode;
        if (this->endNode == otherEdge.endNode)
            return this->startNode < otherEdge.startNode;

        // Если ни один из указателей не совпал, сравниваем по первому
        return (this->startNode->name < otherEdge.startNode->name);
    }

    // Оператор сравнения. Нужен чтобы проверять равенство двух ребер
    bool operator == (const Edge& otherEdge) const {
        return (this->startNode == otherEdge.startNode && this->endNode == otherEdge.endNode
            || this->startNode == otherEdge.endNode && this->endNode == otherEdge.startNode);
    }

};


// Граф (хранит указатели на вершины и дуги)
struct Graph
{
    set <Node*> nodes;
    set <Edge> edges;

    // Конструктор добавления вершин графа graph с клавиатуры
    Graph(int n) {
        for (int i = 0; i < n; i++) {
            Node* newNode = new Node();
            newNode->name = i;
            nodes.insert(newNode);
        }
    }

    // Функция возвращает вершину графа graph по передаваемому имени name
    Node* getNode(int name)
    {
        for (auto iterNode : nodes) {     //  graph - граф, в котором ведется поиск
            if (iterNode->name == name) {     //  name - имя искомой вершины
                return iterNode;
            }
        }
        return NULL;     // Если вершины с таким именем нет, то возвращает NULL
    }

    // Ввод дуг графа graph с клавиатуры
    void setEdges(int startNodeName, int endNodeName)
    {
        // Получаем ссылки на вершины по их именам
        Node* startNode = getNode(startNodeName);
        Node* endNode = getNode(endNodeName);
        // Если такие вершины существуют, то создаем дугу с указателями на них

        edges.insert(Edge(startNode, endNode));

    }

};

// Являются ли хоть одна пара ребер смежными
bool hasMatchingPoints(vector<Edge> edges)
{
    for (int i = 0; i < edges.size(); i++)
    {
        for (int j = i + 1; j < edges.size(); j++)
        {
            if (edges[i].startNode == edges[j].startNode ||
                edges[i].startNode == edges[j].endNode ||
                edges[i].endNode == edges[j].startNode ||
                edges[i].endNode == edges[j].endNode)
                return true;
        }
    }
    return false;
}

// Функция возвращает все паросочетания относительно переданного ребра
vector<Edge> findAllMatchings(Graph graph, Edge edge)
{
    vector<Edge> mathcings;
    set <Edge> edges = graph.edges; // присвоение графа

    // Перебираем все ребра графа
    for (Edge graphEdge : edges)
    {
        if (!(graphEdge == edge))
        {
            // Попарно проверяем текущее ребро со всеми остальными
            vector<Edge> checkEdges = { graphEdge, edge };

            if (!hasMatchingPoints(checkEdges))
                mathcings.push_back(graphEdge);
        }
    }
    return mathcings;
}


// Функция находит наибольшее паросочетание переданных ребер
vector<Edge> findMaxMatchingFromVector(vector<Edge> edges)
{
    // Если все ребра не смежные
    if (!hasMatchingPoints(edges))
        return edges;

    // Хранилище наибольшего паросочетания
    vector<Edge> maxMatchings;
    for (int i = 0; i < edges.size(); i++)
    {
        // Хранилище ребер, не смежных i-тому
        vector<Edge> newEdges;
        for (int j = i + 1; j < edges.size(); j++)
        {
            vector<Edge> checkEdges = { edges[i], edges[j] };
            // Проверяем i-тое и j-тое ребра на смежность
            if (!hasMatchingPoints(checkEdges))
                newEdges.push_back(edges[j]);
        }

        // Если хоть одно ребро из вектора является не смежным относительно i-того
        if (newEdges.size() > 0)
        {
            // Рекурсивно проверяем не смежные относительно i-того ребра ребра на смежность между друг другом
            vector<Edge> newMaxMatchings = findMaxMatchingFromVector(newEdges);
            newMaxMatchings.push_back(edges[i]);
            // Если количество не смежных ребер оказалось больше чем предыдущее
            if (newMaxMatchings.size() > maxMatchings.size())
                maxMatchings = newMaxMatchings;
        }
        else
        {
            // Если до этого вектор несмежных ребер был пуст
            if (1 > maxMatchings.size())
                maxMatchings = { edges[i] };
        }
    }
    return maxMatchings;
}


// Главная функция - нахождения наибольшего паросочетания в графе
void findMaxMatchingVector(Graph graph)
{
    vector<Edge> maxMatching; // ответ хранится тут
    set <Edge> edges = graph.edges;

    // Перебираем все ребра
    for (Edge graphEdge : edges)
    {
        // Находим все паросочетания отосительно перебираемого
        vector<Edge> mathcings = findAllMatchings(graph, graphEdge);
        // В найденном векторе ищем максимальное паросочетание
        vector<Edge> newMaxMatching = findMaxMatchingFromVector(mathcings);
        // Добавляем перебираемое ребро
        newMaxMatching.push_back(graphEdge);
        // Если новый вектор больше старого по размеру
        if (newMaxMatching.size() > maxMatching.size())
            maxMatching = newMaxMatching;
    }

    // Вывод вектора ребер на экран (В векторе хранится наибольшее паросочетание)
    for (Edge edge : maxMatching) {
        cout << edge.startNode->name << " --- " << edge.endNode->name << endl;
        fout << edge.startNode->name << " --- " << edge.endNode->name << endl;
    }
}

// функция вывода матрицы
void toString(int** Matrix, int n) {
    for (int i = 0; i < n; i++)
    {
        cout << i << " : ";
        fout << i << " : ";
        for (int j = 0; j < n; j++) {
            cout << setw(4) << Matrix[i][j];
            fout << setw(4) << Matrix[i][j];
        }
        cout << endl;
        fout << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    int n;
    string s, ss, menu;
    while (true) {
        cout << "Введите кол-во вершин графа: "; cin >> n;
        Graph graph(n);

        int** Matrix = new int* [n]; ;
        for (int i = 0; i < n; i++) {
            Matrix[i] = new int[n];
            for (int j = 0; j < n; j++)
            {
                Matrix[i][j] = 0;
            }
        }

        cout << "\n Выберете способ построения графа: \n 1) Случайное заполнение \n"
            << " 2) Заполнить самому \n 3) Выход\n Введите: ";
        cin >> menu;
        switch (atoi(menu.c_str()))
        {
        case 1:
            system("cls");
            int r;
            cout << "Введите вероятность присутствия ребра(чем больше, тем выше вероятность): ";
            cin >> r;
            for (int i = 0; i < n; i++)
            {
                for (int j = i + 1; j < n; j++)
                {
                    if (1 <= (rand() % r)) {
                        Matrix[i][j] = Matrix[j][i] = 1;
                        graph.setEdges(i, j);
                    }
                }
            }

            cout << "-----------------" << endl << "Матрица смежности: \n";
            fout << endl << "-----------------" << endl << "Матрица смежности: \n";
            toString(Matrix, n);

            cout << endl << "Наибольшее паросочетание:" << endl;
            fout << endl << "Наибольшее паросочетание:" << endl;
            findMaxMatchingVector(graph);

            system("pause");
            break;
        case 2:
            system("cls");

            cout << "Вводите по две вершины через пробел" << endl
                << "Для выхода введите *" << endl
                << "Пример: \n 0 1\n 0 2\n ....\nВведите:\n";

            while (true) {

                cin >> s >> ss;
                if (s == "*" || ss == "*") break;
                if (stoi(s) >= n || stoi(ss) >= n) cout << "Введена некорректная вершина!" << endl;
                else {
                    Matrix[stoi(s)][stoi(ss)] = Matrix[stoi(ss)][stoi(s)] = 1;
                    graph.setEdges(stoi(s), stoi(ss));
                }
            }
            cout << "-----------------" << endl << "Матрица смежности: \n";
            fout << endl << "-----------------" << endl << "Матрица смежности: \n";
            toString(Matrix, n);

            cout << "Наибольшее паросочетание:" << endl;
            fout << endl << "Наибольшее паросочетание:" << endl;
            findMaxMatchingVector(graph);

            system("pause");
            break;
        case 3:
            exit(0);
            break;
        }
        for (int i = 0; i < n; i++)
            delete[] Matrix[i];
        delete[] Matrix;
        system("cls");
    }
    fout.close();
    return 0;
}