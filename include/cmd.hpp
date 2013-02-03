/*! $file: cmd.hpp
 *
 *  (c) 2013 Michael Small <michaelsmall@me.com>
 *
 *  This work is licensed under a Creative Commons Attribution-ShareAlike 3.0
 *  Unported License.
 *
 *  This license permits you to share, adapt and make commercial use of this
 *  work given you meet certain requirements.
 *
 *  For information regarding this license, please visit:
 *    -> http://creativecommons.org/licenses/by-sa/3.0/deed.en_US
 */

#ifndef CMD_HPP
#define CMD_HPP 1

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/function.hpp>

#include <string>
#include <set>

namespace cmd {

    namespace qi = boost::spirit::qi;

    /*! Attribute definitions for command items.
     */
    enum cmd_attr {
        flag=0x01,///< The item is a flag
        label=0x02,///< The item is a label
        required=0x04,///< The item is required
        optional=0x08///< The item is optional
    };

    class cmd_apply {
        boost::function<void()> fn;
    public:
        cmd_apply() {}
        void operator()(const std::string&, qi::unused_type,
                qi::unused_type) const {
            fn();
        }
        typedef void result_type;
    };

    /*! An command line item.
     */
    struct cmd_item {
        unsigned attr;///< Attribute bitset
        char short_key;///< Short option (e.g. -x)
        std::string long_key;///< Long option (e.g. --xyz)
        cmd_apply apply;///< Successful parse callback
        std::string descr;///< Description of the item
    };

    /*! Parser for command item.
     */
    class cmd_parser
        : qi::grammar<std::string::const_iterator, std::string(),
        qi::space_type> {
        qi::rule<std::string::const_iterator, std::string(),
            qi::space_type> start;
    public:
        cmd_parser(cmd_item& ci)
            : cmd_parser::base_type(start) {
            qi::rule<iterator_type, std::string(), skipper_type> s, l;
            if(ci.attr & flag) {
                if(ci.short_key) {
                    s = -qi::lit("-" + ci.short_key);
                }
                if(!ci.long_key.empty()) {
                    l = -qi::lit("--" + ci.long_key);
                }
            } else {
                //TODO: This
                //l = (ci.attr & optional) ? +qi::char_ : -(+qi::char_);
            }
            start = (s[ci.apply] >> l[ci.apply]);
        }
    };


    /*! Encapsulate item data and provide parser instance.
     */
    struct cmd_item_wrapper {
        cmd_item data;///< Item information
        cmd_parser parser;///< Item parser

        /*! Construct wrapper from an item instance.
         */
        explicit cmd_item_wrapper(const cmd_item& d)
            : data(d), parser(data) {}

        /*! Dereference item data.
         */
        const cmd_item* operator -> () const {
            return &data;
        }

        cmd_item* operator -> () {
            return &data;
        }
    };

    /*! Comparator for cmd_item types.
     */
    struct cmp_cmd_item {
        bool operator () (const cmd_item_wrapper& lhs,
                const cmd_item_wrapper& rhs) {
            if(lhs->attr & flag) {
                if(rhs->attr & flag) {
                    return lhs->short_key < rhs->short_key;
                }
                return true;
            } else if(rhs->attr & flag) {
                return false;
            }
            return lhs->long_key < rhs->long_key;
        }
    };

    /*! Convenience type for storing command items.
     */
    typedef std::set<cmd_item_wrapper, cmp_cmd_item> cmd_set;

}//namespace cmd

#endif//CMD_HPP
