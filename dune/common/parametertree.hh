// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_PARAMETERTREE_HH
#define DUNE_PARAMETERTREE_HH

#include <iostream>
#include <istream>
#include <map>
#include <ostream>
#include <string>
#include <typeinfo>
#include <vector>

#include <dune/common/fvector.hh>

namespace Dune {

  /** \brief Hierarchical structure of string parameters
   * \ingroup Common
   */
  class ParameterTree
  {
    // class providing a single static parse() function, used by the
    // generic get() method
    template<typename T>
    struct Parser;

  public:

    typedef std::vector<std::string> KeyVector;

    /** \brief Create new ConfigParser
     */
    ParameterTree();


    /** \brief test for key
     *
     * Tests wether given key exists.
     *
     * \param key key name
     * \return true if key exists in structure, otherwise false
     */
    bool hasKey(const std::string& key) const;


    /** \brief test for substructure
     *
     * Tests wether given substructure exists.
     *
     * \param sub substructure name
     * \return true if substructure exists in structure, otherwise false
     */
    bool hasSub(const std::string& sub) const;


    /** \brief get value reference for key
     *
     * Returns refference to value for given key name.
     * This creates the key, if not existent.
     *
     * \param key key name
     * \return reference to coresponding value
     */
    std::string& operator[] (const std::string& key);


    /** \brief print structure to std::cout
     */
    void report() const;


    /** \brief print distinct substructure to std::cout
     *
     * Prints all entries with given prefix.
     *
     * \param prefix for key and substructure names
     */
    void report(const std::string prefix) const;


    /** \brief print distinct substructure to stream
     *
     * Prints all entries with given prefix.
     *
     * \param stream Stream to print to
     * \param prefix for key and substructure names
     */
    void reportStream(std::ostream& stream,
                      const std::string& prefix = "") const;


    /** \brief get substructure by name
     *
     * \param sub substructure name
     * \return reference to substructure
     */
    ParameterTree& sub(const std::string& sub);


    /** \brief get const substructure by name
     *
     * \param sub substructure name
     * \return reference to substructure
     */
    const ParameterTree& sub(const std::string& sub) const;


    /** \brief get value as string
     *
     * Returns pure string value for given key.
     *
     * \param key key name
     * \param defaultValue default if key does not exist
     * \return value as string
     */
    std::string get(const std::string& key, const std::string& defaultValue);

    /** \brief get value as string
     *
     * Returns pure string value for given key.
     *
     * \todo This is a hack so get("my_key", "xyz") compiles
     * (without this method "xyz" resolves to bool instead of std::string)
     * \param key key name
     * \param defaultValue default if key does not exist
     * \return value as string
     */
    std::string get(const std::string& key, const char* defaultValue);


    /** \brief get value as int
     *
     * Returns value for given key interpreted as int.
     *
     * \param key key name
     * \param defaultValue default if key does not exist
     * \return value as int
     */
    int get(const std::string& key, int defaultValue);


    /** \brief get value as double
     *
     * Returns value for given key interpreted as double.
     *
     * \param key key name
     * \param defaultValue default if key does not exist
     * \return value as double
     */
    double get(const std::string& key, double defaultValue);


    /** \brief get value as bool
     *
     * Returns value for given key interpreted as bool.
     *
     * \param key key name
     * \param defaultValue default if key does not exist
     * \return value as bool, false if values = '0', true if value = '1'
     */
    bool get(const std::string& key, bool defaultValue);


    /** \brief get value converted to a certain type
     *
     * Returns value as type T for given key.
     *
     * \tparam T type of returned value.
     * \param key key name
     * \param defaultValue default if key does not exist
     * \return value converted to T
     */
    template<typename T>
    T get(const std::string& key, const T& defaultValue) {
      if(hasKey(key))
        return get<T>(key);
      else
        return defaultValue;
    }

    /** \brief Get value
     *
     * \tparam T Type of the value
     * \param key Key name
     * \throws RangeError if key does not exist
     * \throws NotImplemented Type is not supported
     * \return value as T
     */
    template <class T>
    T get(const std::string& key) {
      if(not hasKey(key))
        DUNE_THROW(RangeError, "Key '" << key << "' not found in parameter "
                   "file!");
      try {
        return Parser<T>::parse((*this)[key]);
      }
      catch(const RangeError&) {
        DUNE_THROW(RangeError, "Cannot parse value \"" <<
                   (*this)[key] << "\" for key \"" << key << "\" as a " <<
                   typeid(T).name());
      }
    }

    /** \brief get value keys
     *
     * Returns a vector of all keys associated to (key,values) entries in
     * order of appearance
     *
     * \return reference to entry vector
     */
    const KeyVector& getValueKeys() const;


    /** \brief get substructure keys
     *
     * Returns a vector of all keys associated to (key,substructure) entries
     * in order of appearance
     *
     * \return reference to entry vector
     */
    const KeyVector& getSubKeys() const;

  protected:
    KeyVector valueKeys;
    KeyVector subKeys;

    std::map<std::string, std::string> values;
    std::map<std::string, ParameterTree> subs;
    static std::string ltrim(const std::string& s);
    static std::string rtrim(const std::string& s);
  };

  template<typename T>
  struct ParameterTree::Parser {
    static T parse(const std::string& str) {
      T val;
      std::istringstream s(str);
      s >> val;
      if(!s)
        DUNE_THROW(RangeError, "Cannot parse value \"" << str << "\" as a " <<
                   typeid(T).name());
      T dummy;
      s >> dummy;
      // now extraction should have failed, and eof should be set
      if(not s.fail() or not s.eof())
        DUNE_THROW(RangeError, "Cannot parse value \"" << str << "\" as a " <<
                   typeid(T).name());
      return val;
    }
  };

  // "How do I convert a string into a wstring in C++?"  "Why, that very simple
  // son.  You just need a these hundred lines of code."
  // Instead im gonna restrict myself to string with charT=char here
  template<typename traits, typename Allocator>
  struct ParameterTree::Parser<std::basic_string<char, traits, Allocator> > {
    static std::basic_string<char, traits, Allocator>
    parse(const std::string& str) {
      std::string trimmed = ltrim(rtrim(str));
      return std::basic_string<char, traits, Allocator>(trimmed.begin(),
                                                        trimmed.end());
    }
  };

  template<typename T, int n>
  struct ParameterTree::Parser<FieldVector<T, n> > {
    static FieldVector<T, n>
    parse(const std::string& str) {
      FieldVector<T, n> val;
      std::istringstream s(str);
      for(int i = 0; i < n; ++i) {
        s >> val[i];
        if(!s)
          DUNE_THROW(RangeError, "Cannot parse value \"" << str << "\" as a "
                     "FieldVector<" << typeid(T).name() << ", " << n << ">");
      }
      T dummy;
      s >> dummy;
      // now extraction should have failed, and eof should be set
      if(not s.fail() or not s.eof())
        DUNE_THROW(RangeError, "Cannot parse value \"" << str << "\"  as a "
                   "FieldVector<double, " << n << ">");
      return val;
    }
  };

  template<typename T, typename A>
  struct ParameterTree::Parser<std::vector<T, A> > {
    static std::vector<T, A>
    parse(const std::string& str) {
      std::vector<T, A> vec;
      std::istringstream s(str);
      while(true) {
        T val;
        s >> val;
        if(s.fail()) {
          if(s.eof())
            // extraction failed because of EOF, that OK
            break;
          // otherwise, it failed because of something else
          DUNE_THROW(RangeError, "Cannot parse value \"" << str << "\" as a "
                     "std::vector<" << typeid(T).name() << ", " <<
                     typeid(A).name() << ">");
        }
        vec.push_back(val);
      }
      return vec;
    }
  };

} // end namespace Dune

#endif // DUNE_PARAMETERTREE_HH