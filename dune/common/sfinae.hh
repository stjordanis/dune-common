#ifndef DUNE_SFINAE_HH
#define DUNE_SFINAE_HH

namespace Dune {

    /**
     * @addtogroup Typetraits
     * @brief provide yes/no type with different size to
     *        help implementing typetraits
     */
    struct SFINAEBase
    {
        struct yes { char dummy[1]; };
        struct no  { char dummy[2]; };
    };
}

#endif // DUNE_SFINAE_HH
