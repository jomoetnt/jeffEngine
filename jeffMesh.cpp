#include "jeffMesh.h"

using namespace jeffNamespace;

void jeffMesh::loadFromObj(const char* filename)
{
    std::ifstream file(filename);

    std::string line;

    if (file.is_open())
    {
        int i = 0;

        while (std::getline(file, line))
        {
            objProcessLine(line, i);
        }

        file.close();
    }
    else
    {
        throw std::runtime_error("could not open obj");
    }
}

void jeffMesh::objProcessLine(std::string line, int& i)
{
    char firstChar = line.c_str()[0];
    char secondChar = line.c_str()[1];

    // Ignore comments (in obj)
    if (firstChar == '#')
        return;

    // Get object name
    if (firstChar == 'o')
    {
        name = line.erase(0, 2);
        return;
    }

    // Set smooth shading
    if (firstChar == 's')
    {
        line = line.erase(0, 2);
        if (line.front() == '0')
            smoothShading = false;
        else if (line.front() == '1')
            smoothShading = true;
        return;
    }

    if (firstChar == 'v')
    {
        // Add vertex
        if (secondChar == ' ')
        {
            line = line.erase(0, 2);
            std::vector<std::string> components = jeffHelper::split(line, " ");

            DirectX::XMFLOAT3 vpos = DirectX::XMFLOAT3(std::stof(components.data()[0]), std::stof(components.data()[1]), std::stof(components.data()[2]));
            vpositions.emplace_back(vpos);
            return;
        }
        // Add vertex normal
        if (secondChar == 'n')
        {
            line = line.erase(0, 3);
            std::vector<std::string> components = jeffHelper::split(line, " ");

            DirectX::XMFLOAT3 vnorm = DirectX::XMFLOAT3(std::stof(components.data()[0]), std::stof(components.data()[1]), std::stof(components.data()[2]));
            vnormals.emplace_back(vnorm);
            return;
        }
        // Add vertex texture coordinate
        if (secondChar == 't')
        {
            line = line.erase(0, 3);
            std::vector<std::string> components = jeffHelper::split(line, " ");

            DirectX::XMFLOAT2 vtex = DirectX::XMFLOAT2(std::stof(components.data()[0]), std::stof(components.data()[1]));
            vtexcoords.emplace_back(vtex);
            return;
        }
    }

    // Add face
    if (firstChar == 'f')
    {
        handleFace(line, i);
        return;
    }
}

void jeffMesh::handleFace(std::string line, int& i)
{
    // Split up slashes and spaces - obj format is (for example) 5/1/1 3/2/5 4/3/2
    line = line.erase(0, 2);
    std::vector<std::string> components = jeffHelper::split(line, " ");

    std::vector<std::string> indices1 = jeffHelper::split(components.data()[0], "/");
    std::vector<std::string> indices2 = jeffHelper::split(components.data()[1], "/");
    std::vector<std::string> indices3 = jeffHelper::split(components.data()[2], "/");

    jeffVertex vert1{};
    jeffVertex vert2{};
    jeffVertex vert3{};

    // Subtract 1 because obj is not zero-indexed
    int vindex1 = std::stoi(indices1.data()[0]) - 1;
    int vindex2 = std::stoi(indices2.data()[0]) - 1;
    int vindex3 = std::stoi(indices3.data()[0]) - 1;

    vert1.position = DirectX::XMFLOAT3(vpositions.data()[vindex1]);
    vert2.position = DirectX::XMFLOAT3(vpositions.data()[vindex2]);
    vert3.position = DirectX::XMFLOAT3(vpositions.data()[vindex3]);

    vindex1 = std::stoi(indices1.data()[1]) - 1;
    vindex2 = std::stoi(indices2.data()[1]) - 1;
    vindex3 = std::stoi(indices3.data()[1]) - 1;

    vert1.texcoord = DirectX::XMFLOAT2(vtexcoords.data()[vindex1]);
    vert2.texcoord = DirectX::XMFLOAT2(vtexcoords.data()[vindex2]);
    vert3.texcoord = DirectX::XMFLOAT2(vtexcoords.data()[vindex3]);

    vindex1 = std::stoi(indices1.data()[2]) - 1;
    vindex2 = std::stoi(indices2.data()[2]) - 1;
    vindex3 = std::stoi(indices3.data()[2]) - 1;

    vert1.normal = DirectX::XMFLOAT3(vnormals.data()[vindex1]);
    vert2.normal = DirectX::XMFLOAT3(vnormals.data()[vindex2]);
    vert3.normal = DirectX::XMFLOAT3(vnormals.data()[vindex3]);

    DirectX::XMFLOAT3 gray = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    vert1.colour = DirectX::XMFLOAT3(gray);
    vert2.colour = DirectX::XMFLOAT3(gray);
    vert3.colour = DirectX::XMFLOAT3(gray);

    vertices.emplace_back(vert1);
    vertices.emplace_back(vert2);
    vertices.emplace_back(vert3);

    // Determine winding order
    DirectX::XMVECTOR vertex1 = DirectX::XMLoadFloat3(&vert1.position);
    DirectX::XMVECTOR vertex2 = DirectX::XMLoadFloat3(&vert2.position);
    DirectX::XMVECTOR vertex3 = DirectX::XMLoadFloat3(&vert3.position);
    DirectX::XMVECTOR normal1 = DirectX::XMLoadFloat3(&vert1.normal);
    DirectX::XMVECTOR normal2 = DirectX::XMLoadFloat3(&vert2.normal);
    DirectX::XMVECTOR normal3 = DirectX::XMLoadFloat3(&vert3.normal);

    DirectX::XMVECTOR faceNorm = DirectX::XMVectorAdd(normal1, normal2);
    faceNorm = DirectX::XMVectorAdd(faceNorm, normal3);
    faceNorm = DirectX::XMVectorScale(faceNorm, 1 / 3.0f);

    DirectX::XMVECTOR leg1 = DirectX::XMVectorSubtract(vertex2, vertex1);
    DirectX::XMVECTOR leg2 = DirectX::XMVectorSubtract(vertex3, vertex1);

    DirectX::XMVECTOR computedNormal = DirectX::XMVector3Cross(leg1, leg2);
    DirectX::XMVECTOR alignment = DirectX::XMVector3Dot(computedNormal, faceNorm);

    DirectX::XMFLOAT3 agreement;
    DirectX::XMStoreFloat3(&agreement, alignment);

    if (agreement.x < 0.0f)
    {
        indices.emplace_back(i++);
        indices.emplace_back(i++);
        indices.emplace_back(i++);
    }
    else
    {
        indices.emplace_back(i++);
        indices.emplace_back(++i);
        indices.emplace_back(i++ - 1);
    }
}