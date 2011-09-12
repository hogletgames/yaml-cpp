#ifndef NODE_CONVERT_H_62B23520_7C8E_11DE_8A39_0800200C9A66
#define NODE_CONVERT_H_62B23520_7C8E_11DE_8A39_0800200C9A66

#if defined(_MSC_VER) || (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ >= 4)) // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif


#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/iterator.h"
#include <map>
#include <sstream>

namespace YAML
{
	// std::string
	template<>
	struct convert<std::string> {
		static Node encode(const std::string& rhs) {
			return Node(rhs);
		}
		
		static bool decode(const Node& node, std::string& rhs) {
			if(node.Type() != NodeType::Scalar)
				return false;
			rhs = node.scalar();
			return true;
		}
	};
	
#define YAML_DEFINE_CONVERT_STREAMABLE(type)\
	template<>\
	struct convert<type> {\
		static Node encode(const type& rhs) {\
			std::stringstream stream;\
			stream << rhs;\
			return Node(stream.str());\
		}\
		\
		static bool decode(const Node& node, type& rhs) {\
			if(node.Type() != NodeType::Scalar)\
				return false;\
			std::stringstream stream(node.scalar());\
			stream >> rhs;\
			return !!stream;\
		}\
	}
	
	YAML_DEFINE_CONVERT_STREAMABLE(int);
	YAML_DEFINE_CONVERT_STREAMABLE(unsigned);
	YAML_DEFINE_CONVERT_STREAMABLE(short);
	YAML_DEFINE_CONVERT_STREAMABLE(unsigned short);
	YAML_DEFINE_CONVERT_STREAMABLE(long);
	YAML_DEFINE_CONVERT_STREAMABLE(unsigned long);
	YAML_DEFINE_CONVERT_STREAMABLE(long long);
	YAML_DEFINE_CONVERT_STREAMABLE(unsigned long long);
	
	YAML_DEFINE_CONVERT_STREAMABLE(char);
	YAML_DEFINE_CONVERT_STREAMABLE(unsigned char);
	
	YAML_DEFINE_CONVERT_STREAMABLE(float);
	YAML_DEFINE_CONVERT_STREAMABLE(double);
	YAML_DEFINE_CONVERT_STREAMABLE(long double);
	
#undef YAML_DEFINE_CONVERT_STREAMABLE

	template<typename K, typename V>
	struct convert<std::map<K, V> > {
		static Node encode(const std::map<K, V>& rhs) {
			Node node(NodeType::Map);
			for(typename std::map<K, V>::const_iterator it=rhs.begin();it!=rhs.end();++it)
				node[it->first] = it->second;
			return node;
		}
		
		static bool decode(const Node& node, std::map<K, V>& rhs) {
			rhs.clear();
			return false;
		}
	};
	
	template<typename T>
	struct convert<std::vector<T> > {
		static Node encode(const std::vector<T>& rhs) {
			Node node(NodeType::Sequence);
			for(std::size_t i=0;i<rhs.size();i++)
				node.append(rhs[i]);
			return node;
		}
		
		static bool decode(const Node& node, std::vector<T>& rhs) {
			if(node.Type() != NodeType::Sequence)
				return false;
			
			rhs.clear();
			for(const_iterator it=node.begin();it!=node.end();++it)
				rhs.push_back(it->as<T>());
			return true;
		}
	};
}

#endif // NODE_CONVERT_H_62B23520_7C8E_11DE_8A39_0800200C9A66
