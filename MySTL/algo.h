#pragma once
#include "algobase.h"
namespace stl
{

    template <class BidirectionalIter>
    void reverse_dispatch(BidirectionalIter first, BidirectionalIter last,
        bidirectional_iterator_tag)
    {
        while (true)
        {
            if (first == last || first == --last)
                return;
            stl::iter_swap(first++, last);
        }
    }
    template <class RandomIter>
    void reverse_dispatch(RandomIter first, RandomIter last,
        random_access_iterator_tag)
    {
        while (first < last)
            stl::iter_swap(first++, --last);
    }

    template <class BidirectionalIter>
    void reverse(BidirectionalIter first, BidirectionalIter last)
    {
        stl::reverse_dispatch(first, last, iterator_category(first));
    }
}