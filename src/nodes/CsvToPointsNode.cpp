#include "CsvToPointsNode.h"

REGISTER_NODE(CsvToPointsNode, "CSV to points");

std::vector<std::array<float,3>> parseCsv(const std::string& csv);

CsvToPointsNode::CsvToPointsNode() {
  addInputPin<RuntimeType::String>("CSV");
  addOutputPin<RuntimeType::PointVec>("Points");
}

void CsvToPointsNode::update() {
  this->wait_for_input();
  auto csv = readPin<RuntimeType::String>(0);

  if (csv) {
    auto points = parseCsv(*csv);
    writePin<RuntimeType::PointVec>(
      0,
      std::make_shared<typename RuntimeType::PointVec::type>(std::move(points))
    );
  } else {
    resetPin<RuntimeType::PointVec>(0);
  }
}

std::vector<std::array<float,3>> parseCsv(const std::string& csv) {
  std::vector<std::array<float,3>> result;
  std::stringstream stream(csv);

  std::string line;
  while (std::getline(stream, line)) {
    std::array<float, 3> lineResult = {0};
    std::stringstream lineStream(line);
    std::string value;

    int i = 0;
    while (std::getline(lineStream, value, ',')) {
      std::stringstream valueStream(value);
      valueStream >> lineResult[i];
      i++;
      if (i == 3) break;
    }

    result.push_back(std::move(lineResult));
  }

  return result;
}
