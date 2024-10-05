/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BilateralMap.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 14:54:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 15:56:41 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BILATERALMAP_TPP

# define BILATERALMAP_TPP

#include <map>
#include <cstddef>

/*

    Boths keys and values have to be totally unique

*/


template <typename Key, 
            typename Value, 
            template <typename> class MemPool, 
            typename Keyless = std::less<Key>, 
            typename Valueless = std::less<Value>
>

class BilateralMap
{
    public:

        typedef typename std::map<Key, Value*, Keyless, MemPool<std::pair<const Key, const Value*> > >   MapKeyToValue;
        typedef typename std::map<Value, Key*, Valueless, MemPool<std::pair<const Value, const Key*> > >   MapValueToKey;

        typedef typename MapKeyToValue::iterator Key_Iter;
        typedef typename MapValueToKey::iterator Value_Iter;

        BilateralMap() : _size(0) {}
        ~BilateralMap() {}

        void    insert(const Key& key, const Value& value)
        {
            typename std::map<Key, Value*, Keyless, MemPool<std::pair<const Key, const Value*> > >::iterator 
            key_it = _keyToValue.insert(std::make_pair(key, NULL));


            const Key *key_ptr = &(key_it->first);

            

            typename std::map<Value, Key*, Valueless, MemPool<std::pair<const Value, const Key*> > >::iterator 
            val_it = _ValueToKey.insert(std::make_pair(value, key_ptr)); 

            const Value *val_ptr = &(val_it->first);        

            key_it->second = const_cast<Value*>(val_ptr);
        }

        typename MapKeyToValue::iterator getValue(const Key& key)
        {
            return (_keyToValue.find(key));
        }
        typename MapValueToKey::iterator getKey(const Value& value)
        {
            return (_ValueToKey.find(value));
        }


    private:



        MapKeyToValue  _keyToValue;
        MapValueToKey  _ValueToKey;

        size_t  _size;
};



#endif