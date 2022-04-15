#ifndef UTILS_H
#define UTILS_H
#include <Rcpp.h>
#include <google/protobuf/message.h>

using namespace Rcpp;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::Reflection;


template<class Message>
class MessageVectorWrapper {
  public:
    MessageVectorWrapper(std::vector<std::shared_ptr<Message>>* message_ptr_vector)
      : message_ptr_vector_(message_ptr_vector),
        is_ptr_(true) {}

    MessageVectorWrapper(std::vector<Message>* message_vector)
      : message_vector_(message_vector),
        is_ptr_(false) {}

    Message GetMessage(int idx) {
      if (is_ptr_) {
        return (*(*message_ptr_vector_)[idx].get());
      } else {
        return (*message_vector_)[idx];
      }
    }

    size_t GetSize() {
      if (is_ptr_) {
        return message_ptr_vector_->size();
      } else {
        return message_vector_->size();
      }
    }

  std::vector<std::shared_ptr<Message>>* message_ptr_vector_;
  std::vector<Message>* message_vector_;
  bool is_ptr_;
};

template<class Message>
DataFrame _convert(MessageVectorWrapper<Message> vector_wrapper) {
  DataFrame df = DataFrame::create();
  const Descriptor* descriptor = Message::descriptor();
  const Reflection* reflection = Message::GetReflection();
  for (int i = 0; i < descriptor->field_count(); ++i) {
    const FieldDescriptor* field = descriptor->field(i);

    FieldDescriptor::Type type = field->type();
    std::string name = field->name();

    if (type == FieldDescriptor::Type::TYPE_STRING) {
      CharacterVector vec = {};
      for (size_t i = 0; i < vector_wrapper.GetSize(); ++i) {
        Message message = vector_wrapper.GetMessage(i);
        std::string value = reflection->GetString(message, field);
        vec.push_back(value);
      }
      df.push_back(vec, name);
    } else if (type == FieldDescriptor::Type::TYPE_INT32) {
      NumericVector vec = {};
      for (size_t i = 0; i < vector_wrapper.GetSize(); ++i) {
        Message message = vector_wrapper.GetMessage(i);
        int32_t value = reflection->GetInt32(message, field);
        vec.push_back(value);
      }
      df.push_back(vec, name);
    } else if (type == FieldDescriptor::Type::TYPE_FLOAT) {
      NumericVector vec = {};
      for (size_t i = 0; i < vector_wrapper.GetSize(); ++i) {
        Message message = vector_wrapper.GetMessage(i);
        float value = reflection->GetFloat(message, field);
        vec.push_back(value);
      }
      df.push_back(vec, name);
    }
  }
  return df;
}

template<class Message>
DataFrame convert_ptr_to_dataframe(std::vector<std::shared_ptr<Message>> message_vector) {
  MessageVectorWrapper<Message> vector_wrapper(&message_vector);
  return _convert<Message>(vector_wrapper);
}

template<class Message>
DataFrame convert_to_dataframe(std::vector<Message> message_vector) {
  MessageVectorWrapper<Message> vector_wrapper(&message_vector);
  return _convert<Message>(vector_wrapper);
}
#endif
