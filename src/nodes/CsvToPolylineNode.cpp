#include "CsvToPolylineNode.h"

REGISTER_NODE(CsvToPolylineNode, "CSV to polyline");

std::vector<Vec3> parseCsv(const std::string& csv);

CsvToPolylineNode::CsvToPolylineNode() {
  addInputPin<RuntimeType::String>("CSV");
  addOutputPin<RuntimeType::Polyline>("Polyline");
}

void CsvToPolylineNode::update() {
  this->wait_for_input();
  auto csv = readPin<RuntimeType::String>(0);

  if (!csv) {
    this->resetPin<RuntimeType::Polyline>(0);
    return;
  }

  auto polyline = Polyline( std::move(parseCsv(*csv)) );
  writePin<RuntimeType::Polyline>(
    0,
    std::make_shared<Polyline>(std::move(polyline))
  );
}

std::vector<Vec3> parseCsv(const std::string& csv) {
  std::vector<Vec3> result;
  std::stringstream stream(csv);

  std::string line;
  float lineResult[3];
  while (std::getline(stream, line)) {
    for (int i = 0; i < 3; i++) lineResult[i] = 0;

    std::stringstream lineStream(line);
    std::string value;

    int i = 0;
    while (std::getline(lineStream, value, ',')) {
      std::stringstream valueStream(value);
      valueStream >> lineResult[i];
      i++;
      if (i == 3) break;
    }

    result.emplace_back(lineResult[0], lineResult[1], lineResult[2]);
  }

  return result;
}
