#include "Node.h"
#include <fstream>
#include <ostream>

Node::Node(std::string name)
    : tagName(name)
{}

Node::~Node()
{
    deleteCildren();
}

void Node::addChild(Node *child)
{
    children.push_back(child);
}

Node *Node::removeChild()
{
    Node *child = children.back();
    children.pop_back();
    return child;
}

void Node::printNode(std::ofstream &os, int level, FileType File)
{
    if (tagName.back() == '/') {
        tagName.pop_back();
    }
    if (tagName.back() == ' ') {
        tagName.pop_back();
    }
    for (int i = 0; i < level; i++) {
        std::cout << "  ";
    }
    std::cout << tagName << std::endl;

    for (Node *child : children) {
        child->printNode(os, level + 1, File);
    }
    /* for(int i = 0; i<level; i++){
        std::cout<<"  ";
    }
    std::cout << "</" << tagName << ">" << std::endl; */
}

void Node::printNodeInFile(std::ofstream &of, int level, FileType file)
{
    if (tagName.empty()) {
        return;
    }
    if (tagName.back() == '/') {
        tagName.pop_back();
    }
    if (tagName.back() == ' ') {
        tagName.pop_back();
    }

    switch (file) {
    case JSON: {
        for (int i = 0; i < level; i++) {
            of << "  ";
        }
        // name-Block
        of << "{\n";
        for (int i = 0; i < level + 1; i++) {
            of << "  ";
        }
        std::string name = diffuseTagName(tagName);
        of << "\"name\": \"" << name << "\"";
        for (auto &[key, value] : Attribute) {
            of << ",\n";
            for (int i = 0; i < level + 1; i++)
                of << "  ";
            of << "\"" << key << "\": \"" << value << "\"";
        }
        // children-Block
        if (children.empty()) {
            of << "\n";
            for (int i = 0; i < level; i++) {
                of << "  ";
            }
            of << "}";
        } else {
            of << ",\n";
            for (int i = 0; i < level + 1; i++) {
                of << "  ";
            }
            of << "\"children\": [\n";
            for (int i = 0; i < children.size(); i++) {
                children[i]->printNodeInFile(of, level + 1, file);
                if (i < children.size() - 1) {
                    of << ",\n";
                } else {
                    of << "\n";
                }
            }
            for (int i = 0; i < level + 1; i++) {
                of << "  ";
            }
            of << "]\n";
            for (int i = 0; i < level; i++) {
                of << "  ";
            }
            of << "}";
        }
    } break;

    case XML: {
        std::string name = diffuseTagName(tagName);

        for (int i = 0; i < level; i++) {
            of << "  ";
        }
        // name-Block
        if (name[0] == ' ') {
            name.erase(name.begin());
        }
        if (!children.empty()){
            of << "<" << name;
        }
        else {
            for (int i = 0; i < level-1; i++) {
                of << "  ";
            }
            of << "<" << name;
        }

        if (!this->Attribute.empty()) {
            for (auto &[key, value] : Attribute) {
                of << key << "=\"" << value << "\"";
            }
        }
        // children-Block
        if (children.empty()) {
            of << "/>\n";
            for (int i = 0; i < level-2; i++) {
                of << "  ";
            }
        } else {
            of << ">\n";
            for (int i = 0; i < level + 1; i++) {
                of << "  ";
            }
            for (int i = 0; i < children.size(); i++) {
                children[i]->printNodeInFile(of, level + 1, file);
            }
            for (int i = 0; i < level + 1; i++) {
                of << "  ";
            }

            of << "</" << name << ">\n";
        }
    }break;
    case NONE: {
        std::cout << "keine Datei" << std::endl;
        break;
    }

    default:
        break;
    }
}

void Node::deleteCildren()
{
    for (Node *child : children) {
        delete child;
    }
}

void Node::setAttribute(const std::string &name, const std::string &value)
{
    this->Attribute[name] = value;
}

void Node::removeAttribute(const std::string &name)
{
    this->Attribute.erase(name);
}

bool Node::hasAttribute(const std::string &name)
{
    if (this->Attribute.find(name) == this->Attribute.end()) {
        return false;
    }
    return true;
}

std::string Node::getAttribute(const std::string &name)
{
    return Attribute.at(name);
}

int Node::getChildLen()
{
    return children.size();
}

std::string Node::diffuseTagName(std::string fullName)
{
    std::string name;

    if (fullName.find(' ') != std::string::npos) {
        if (fullName.back() == '/') {
            fullName.pop_back();
        }
        if (fullName.back() == ' ') {
            fullName.pop_back();
        }
        int i = fullName.find(' ') + 1;
        name = fullName.substr(0, i);
        for (int var = 2; var < fullName.length(); ++var) {
            if (fullName.find('=') != std::string::npos) {
                int j = fullName.find('=', i);
                std::string attributeName = fullName.substr(i, (j - i));
                int k = fullName.find("\"", j + 2);
                std::string attributeValue = fullName.substr((j + 2), (k - j - 2));
                this->setAttribute(attributeName, attributeValue);
                if (fullName.find(' ', k) != std::string::npos) {
                    i = fullName.find(" ", k) + 1;
                }
            }
        }
    } else {
        name = fullName;
    }

    return name;
}
