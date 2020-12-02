#pragma once

#include <fc/variant.hpp>
#include <fc/reflect/variant.hpp>

namespace eosio { namespace jsonrpc {

enum error_code {
   undefined        = 0,
   server_error     = -32000,
   parse_error      = -32700,
   invalid_request  = -32600,
   method_not_found = -32601,
   invalid_params   = -32602,
   internal_error   = -32603,
};

typedef std::function<fc::variant(const fc::variant&)> request_handler;

// TODO: fc::optional is removed in the development version of eosio, replace this with std::optional later.
template <typename T>
using optional = fc::optional<T>;

struct error {
   error(): code( error_code::undefined ) {}

   error( error_code c, std::string m, optional<fc::variant> d = optional<fc::variant>() ): code( c ), message( m ), data( d ) {}

   error_code code;
   std::string message;
   optional<fc::variant> data;
};

struct response {
   std::string jsonrpc = "2.0";
   optional<fc::variant> result;
   optional<error> error;
   fc::variant id;

   operator fc::variant() {
      fc::variant out;
      fc::to_variant( *this, out );
      return out;
   }
};

typedef std::function<fc::variant(const fc::variant&)> request_handler;

namespace detail {
   class endpoint_impl {
      public:
         void rpc_id( const fc::variant_object&, response& );
         void rpc_jsonrpc( const fc::variant_object& request, response& response );
         response rpc( const fc::variant& message );

         void add_handler( const std::string& method_name, request_handler handler );
         fc::variant handle_request( const std::string& message );

      private:
         std::map<std::string,request_handler> handlers;
   };
}

class endpoint {
   public:
      endpoint(): my(new detail::endpoint_impl()) {}

      void add_handler( const std::string& method, request_handler handler );
      fc::variant handle_request( const std::string& message );

   private:
      std::unique_ptr<detail::endpoint_impl> my;
};

} } /// namespace eosio::jsonrpc

FC_REFLECT_ENUM( eosio::jsonrpc::error_code, (undefined)(server_error)(parse_error)(invalid_request)(method_not_found)(invalid_params)(internal_error) )
FC_REFLECT( eosio::jsonrpc::error, (code)(message)(data) )
FC_REFLECT( eosio::jsonrpc::response, (jsonrpc)(result)(error)(id) )
