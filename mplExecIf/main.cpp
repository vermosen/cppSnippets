
#include <iostream>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>

namespace mpl = boost::mpl;

template< bool done = true >
struct exec_if_impl
{
	template<typename Iterator, typename LastIterator, typename Pred, typename Exec>
	static void execute(Iterator*, LastIterator*, Pred const&, Exec const&)
	{
	}
};

template<>
struct exec_if_impl<false>
{
	template<typename Iterator, typename LastIterator, typename Pred, typename Exec>
	static void execute(Iterator*, LastIterator*, Pred const& f, Exec const& e)
	{
		typedef typename mpl::deref<Iterator>::type item;

		if (!f(static_cast<item*>(0)))
		{
			typedef typename mpl::next<Iterator>::type iter;
			exec_if_impl<boost::is_same<iter, LastIterator>::value>
				::execute(static_cast<iter*>(0), static_cast<LastIterator*>(0), f, e);
		}
		else
			e(static_cast<item*>(0));
	}
};

template<typename Sequence, typename Pred, typename Exec>
inline
void exec_if(Pred const& f, Exec const& e, Sequence* = 0)
{
	BOOST_MPL_ASSERT((mpl::is_sequence<Sequence>));

	typedef typename mpl::begin<Sequence>::type first;
	typedef typename mpl::end<Sequence>::type last;

	exec_if_impl<boost::is_same<first, last>::value>
		::execute(static_cast<first*>(0), static_cast<last*>(0), f, e);
}

namespace msg
{
	struct m1 { enum { TYPE = 1 }; static const char* name() { return "m1"; } };
	struct m2 { enum { TYPE = 2 }; static const char* name() { return "m2"; } };
	struct m3 { enum { TYPE = 3 }; static const char* name() { return "m3"; } };
	struct m4 { enum { TYPE = 4 }; static const char* name() { return "m4"; } };
	struct m5 { enum { TYPE = 5 }; static const char* name() { return "m5"; } };
}

struct checker
{
	checker(int chk_type) : type(chk_type) {}

	template <typename Mtype>
	bool operator()(Mtype* = 0) const
	{
		return Mtype::TYPE == type;
	}

	int type;
};

struct exec
{
	template <typename Mtype>
	void operator()(Mtype* = 0) const
	{
		std::cout << Mtype::name() << " executed" << std::endl;
	}
};

int main(void)
{
	typedef mpl::vector<msg::m1, msg::m2, msg::m3, msg::m4, msg::m5> mseq;

	checker chk(3);

	exec_if<mseq>(chk, exec());

	return 0;
}