#include <Rcpp.h>
#include <google/protobuf/message.h>

using namespace Rcpp;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::Reflection;

template<class Message>
DataFrame convert_to_dataframe(std::vector<Message> message_vector) {
  DataFrame df = DataFrame::create();
  const Descriptor* descriptor = Message::descriptor();
  const Reflection* reflection = Message::GetReflection();
  for (int i = 0; i < descriptor->field_count(); ++i) {
    const FieldDescriptor* field = descriptor->field(i);

    FieldDescriptor::Type type = field->type();
    std::string name = field->name();

    if (type == FieldDescriptor::Type::TYPE_STRING) {
      CharacterVector vec = {};
      for (size_t i = 0; i < message_vector.size(); ++i) {
        Message message = message_vector[i];
        std::string value = reflection->GetString(message, field);
        vec.push_back(value);
      }
      df.push_back(vec, name);
    } else if (type == FieldDescriptor::Type::TYPE_INT32) {
      NumericVector vec = {};
      for (size_t i = 0; i < message_vector.size(); ++i) {
        Message message = message_vector[i];
        int32_t value = reflection->GetInt32(message, field);
        vec.push_back(value);
      }
      df.push_back(vec, name);
    }
  }
  return df;
}
