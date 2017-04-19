#include <iostream>
#include <cstring>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/util/json_util.h>
#include "gen/addressbook.pb.h"
#include "gen/school.pb.h"

const int buffsize = 1024;
char buff[buffsize] = {0};

template <typename T>
T* CreateMessage(std::string message) {
  auto msgDes = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(message);
  if (msgDes == NULL) return NULL;

  auto msg = google::protobuf::MessageFactory::generated_factory()->GetPrototype(msgDes);
  if (msg == NULL) return NULL;

  return dynamic_cast<T*>(msg->New());
}

void saveto(google::protobuf::Message* message) {
  printf(" -- %s --\n", __FUNCTION__);
  std::memset(buff, 0, buffsize);
  std::string type_name = message->GetTypeName();
  std::size_t length = sizeof(int) + type_name.size()+1 + message->ByteSize();

  printf("total length:%d [",  length);
  printf("typename length(%d) + typename(%d) + Message(%d)]\n", sizeof(int),type_name.size()+1, message->GetCachedSize());
  printf("typename:%s\n", type_name.c_str());
  printf("typename length:%d\n", type_name.size());
  printf("ByteSize:%d\n", message->ByteSize());
  char* start = buff;
  int* total = (int*)start;

  //total
  *total = length;
  start += sizeof(int);

  //type name length
  total = (int*)start;
  *total = type_name.size()+1;
  start += sizeof(int);

  //type name
  std::memcpy(start, type_name.c_str(), type_name.size());
  start[type_name.size()] = '\0';
  start += type_name.size()+1;

  // message
  bool ret = message->SerializeToArray(start, 1024-(type_name.size() + length + 1));
  printf("saveto %d\n", ret);
}

google::protobuf::Message* readMessage() {
  printf(" -- %s --\n", __FUNCTION__);
  char* start = buff;

  // total
  int total = *((int*)start);
  printf("read total:%d\n", total);
  start += sizeof(int);

  // typename length
  int strlen = *((int*)start);
  start += sizeof(int);
  printf("type name length:%d\n", strlen);


  // typename
  printf("type name:%s\n", start);
  auto msgDes = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(start);
  start += strlen;
  if (msgDes == NULL) return NULL;

  // byte
  int bytesize = total - sizeof(int) - strlen;
  printf("bytesize:%d\n", bytesize);

  auto msg = google::protobuf::MessageFactory::generated_factory()->GetPrototype(msgDes);
  if (msg == NULL) return NULL;

  auto message = msg->New();
  printf("parse %d\n\n", message->ParseFromArray(start, bytesize));
  return message;
}

int main(int argc, char* argv[]) {
  tutorial::Person* person = CreateMessage<tutorial::Person>("tutorial.Person");
  person->set_id(10);
  person->set_email("@gmail.com");
  auto phone = person->add_phones();
  phone->set_number("101");
  std::cout << "person:" << person->DebugString() << std::endl;

  std::string db;
  google::protobuf::util::MessageToJsonString(*person, &db);
  std::cout << "person json:" << db << std::endl;

  std::string printjson;
  google::protobuf::util::JsonOptions options;
  options.always_print_primitive_fields = true;
  options.add_whitespace = true;
  google::protobuf::util::MessageToJsonString(*person, &printjson, options);
  std::cout << "person json full:" << printjson << std::endl;

  tutorial::Person* jsonperson = CreateMessage<tutorial::Person>(person->GetTypeName());
  google::protobuf::util::JsonStringToMessage(db, jsonperson);

  std::cout << "jsonperson:" << jsonperson->DebugString() << std::endl;

  saveto(person);

  auto buffperson = readMessage();
  std::cout << "buffperson:" << buffperson->DebugString() << std::endl;

  return 0;
}