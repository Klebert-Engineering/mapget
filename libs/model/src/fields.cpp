#include "fields.h"

#include <bitsery/bitsery.h>
#include <bitsery/adapter/stream.h>
#include <bitsery/traits/string.h>

namespace mapget
{

Fields::Fields(std::string nodeId) : nodeId_(std::move(nodeId)) {
    addStaticKey(IdStr, "id");
    addStaticKey(TypeIdStr, "typeId");
    addStaticKey(LayerStr, "layer");
    addStaticKey(ChildrenStr, "children");
    addStaticKey(DirectionStr, "direction");
    addStaticKey(ValidityStr, "validity");
    addStaticKey(PropertiesStr, "properties");
}

void Fields::write(std::ostream& outputStream, simfil::FieldId offset) const
{
    // Write the node id which identifies the fields dictionary
    bitsery::Serializer<bitsery::OutputStreamAdapter> s(outputStream);
    s.text1b(nodeId_, std::numeric_limits<uint32_t>::max());
    simfil::Fields::write(outputStream, offset);
}

std::string Fields::readDataSourceNodeId(std::istream& inputStream) {
    // Read the node id which identifies the fields dictionary
    bitsery::Deserializer<bitsery::InputStreamAdapter> s(inputStream);
    std::string fieldsDictNodeId;
    s.text1b(fieldsDictNodeId, std::numeric_limits<uint32_t>::max());
    return fieldsDictNodeId;
}

}
