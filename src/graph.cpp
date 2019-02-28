#include "graph.h"
#include <cmath>
#include <iterator>
#include <fstream>
#include <sstream>

namespace cplex_example {
  Graph::Graph(const char* filename) {
    read_problem(filename);
  }

  void Graph::read_problem(const char* filename) {
    std::ifstream infile(filename);

    std::string line;
    bool is_node_section = false;
    std::uint32_t node_index = 0;

    while (std::getline(infile,line)) {
      if (is_node_section)
      {
        if (node_index >= n_nodes) {
          std::cout << "Found " << node_index << " nodes. Expected " << n_nodes << std::endl;
          return;
        }

        float skip;
        std::stringstream ss(line);
        ss >> skip;
        ss >> x_coord[node_index];
        ss >> y_coord[node_index];

        node_index++;

        if (node_index == n_nodes)
        {
          is_node_section = false;
        }
      }
      else
      {
        std::vector<std::string> column = split(line,' ');

        if (contains(column[0],"DIMENSION"))
        {
          std::istringstream iss(column.back());
          iss >> n_nodes;
          x_coord.assign(n_nodes,0);
          y_coord.assign(n_nodes,0);
        }
        else if (contains(column[0],"NODE_COORD_SECTION"))
        {
            is_node_section = true;
            node_index = 0;
        }
      }
    }

    costs = std::vector<std::vector<float>>(n_nodes, std::vector<float>(n_nodes));

    for(auto i = 0u; i < costs.size(); ++i) {
      costs[i][i] = 999 * n_nodes;
      for(auto j = i + 1; j < costs[i].size(); ++j) {
        const auto dist = std::sqrt(std::pow(x_coord[i] - x_coord[j], 2.0) + std::pow(y_coord[i] - y_coord[j], 2.0));
        costs[i][j] = dist;
        costs[j][i] = dist;
      }
    }
  }
  
  void Graph::print(const char* filename, std::vector<std::uint32_t> sequence) {
    using tinyxml2::XMLDocument;
    using tinyxml2::XMLElement;

    std::cout << "\n\nWrite solution to " << filename << std::endl;
    doc = new XMLDocument();
    XMLElement* root = doc->NewElement("TspSolution");
    XMLElement* element;
    XMLElement* subelement;
    append_xml_key(root, "name", "Test");
    append_xml_key(root, "distanceType", "AIR_DISTANCE");
    append_xml_key(root, "distanceUnitOfMeasurement", "distance");
    element = append_xml_key(root, "locationList");

    // locationList
    for (auto i = 0u; i < n_nodes; i++) {
      subelement = append_xml_key(element, "TspAirLocation");
      set_xml_attrib(subelement,"id",std::to_string(i).c_str());
      append_xml_key(subelement,"id",std::to_string(i).c_str());
      append_xml_key(subelement,"latitude",std::to_string(x_coord[i]).c_str());
      append_xml_key(subelement,"longitude",std::to_string(y_coord[i]).c_str());
    }

    // domicile
    uint32_t offset = n_nodes;
    uint32_t previous_ref = offset;
    element = append_xml_key(root, "domicile");
    set_xml_attrib(element,"id",std::to_string(offset).c_str());
    append_xml_key(element,"id",std::to_string(offset).c_str());
    subelement = append_xml_key(element,"location");
    set_xml_attrib(subelement,"class","TspAirLocation");
    set_xml_attrib(subelement,"reference","0");
    offset++;

    // visitList
    element = append_xml_key(root, "visitList");
    for (auto i = 0u; i < n_nodes; i++) {
      auto index = sequence[i];

      subelement = append_xml_key(element, "Visit");
      set_xml_attrib(subelement,"id",std::to_string(offset+i).c_str());
      append_xml_key(subelement,"id",std::to_string(offset+i).c_str());

      XMLElement* subsubelement = append_xml_key(subelement,"location");
      set_xml_attrib(subsubelement,"class","TspAirLocation");
      set_xml_attrib(subsubelement,"reference",std::to_string(index).c_str());

      subsubelement = append_xml_key(subelement,"previousStandstill");
      set_xml_attrib(subsubelement,"class","Visit");
      set_xml_attrib(subsubelement,"reference",std::to_string(previous_ref).c_str());
      
      previous_ref = offset+i;
    }

    doc->InsertEndChild(root);
    doc->SaveFile(filename);

  }

  tinyxml2::XMLElement* Graph::append_xml_key(tinyxml2::XMLElement* root, const char* key, const char* value) {
    tinyxml2::XMLElement* content;
    content = doc->NewElement(key);
    root->InsertEndChild(content);
    if (value) {
      content->SetText(value);
    }
    return content;
  }

  void Graph::set_xml_attrib(tinyxml2::XMLElement* element, const char* attrib, const char* value)
  {
    element->SetAttribute(attrib, value);
  }

  std::vector<std::string> Graph::split(std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter))
    {
      tokens.push_back(token);
    }
    return tokens;
  }

  bool Graph::contains(std::string str, std::string substr) {
    return (str.find(substr) != std::string::npos);
  }
}