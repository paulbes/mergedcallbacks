#ifndef MERGED_CALLBACKS_HPP
#define MERGED_CALLBACKS_HPP

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <map>
#include <deque>
#include <string>
#include <cassert>


class MergedCallbacks
{
public:

	typedef boost::function<void()> CallbackType;

	enum CallbackPriority
	{
		HIGH=1,
		NORMAL,
		LOW
	};

public:

	MergedCallbacks();

	void executeCallbackWithPriority( CallbackPriority priority );

	/* Static member or functions */

	template<typename R>
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, R (*)(), boost::_bi::list0 > bind_object );

	template<typename R, typename B1, typename A1 >
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, R(*)(B1), boost::_bi::list1<boost::_bi::value<A1> > > bind_object );

	template<typename R, typename B1, typename B2, typename A1, typename A2 >
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, R(*)(B1, B2), boost::_bi::list2<boost::_bi::value<A1>, boost::_bi::value<A2> > > bind_object );

	template<typename R, typename B1, typename B2, typename B3, typename A1, typename A2, typename A3 >
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, R(*)(B1, B2, B3), boost::_bi::list3<boost::_bi::value<A1>, boost::_bi::value<A2>, boost::_bi::value<A3> > > bind_object );

	template<typename R, typename B1, typename B2, typename B3, typename B4, typename A1, typename A2, typename A3, typename A4 >
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, R(*)(B1, B2, B3, B4), boost::_bi::list4<boost::_bi::value<A1>, boost::_bi::value<A2>, boost::_bi::value<A3>, boost::_bi::value<A4> > > bind_object );

	template<typename R, typename B1, typename B2, typename B3, typename B4, typename B5,  typename A1, typename A2, typename A3, typename A4, typename A5 >
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, R(*)(B1, B2, B3, B4, B5), boost::_bi::list5<boost::_bi::value<A1>, boost::_bi::value<A2>, boost::_bi::value<A3>, boost::_bi::value<A4>, boost::_bi::value<A5> > > bind_object );

	/* Virtual or Member functions */

	template<typename R, class T, class A1>
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, boost::_mfi::mf0<R, T>, boost::_bi::list1<boost::_bi::value<A1*> > > bind_object );

	template<typename R, class T, class B1, class A1, class A2>
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, boost::_mfi::mf1<R, T, B1>, boost::_bi::list2<boost::_bi::value<A1*>, boost::_bi::value<A2> > > bind_object );

	template<typename R, class T, class B1, class B2, class A1, class A2, class A3>
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, boost::_mfi::mf2<R, T, B1, B2>, boost::_bi::list3<boost::_bi::value<A1*>, boost::_bi::value<A2>, boost::_bi::value<A3> > > bind_object );

	template<typename R, class T, class B1, class B2, class B3, class A1, class A2, class A3, class A4>
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, boost::_mfi::mf3<R, T, B1, B2, B3>, boost::_bi::list4<boost::_bi::value<A1*>, boost::_bi::value<A2>, boost::_bi::value<A3>, boost::_bi::value<A4> > > bind_object );

	template<typename R, class T,
		class B1, class B2, class B3, class B4,
		class A1, class A2, class A3, class A4, class A5>
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, boost::_mfi::mf4<R, T, B1, B2, B3, B4>,
			boost::_bi::list5<boost::_bi::value<A1*>, boost::_bi::value<A2>, boost::_bi::value<A3>, boost::_bi::value<A4>, boost::_bi::value<A5> > > bind_object );

	template<typename R, class T,
		class B1, class B2, class B3, class B4, class B5,
		class A1, class A2, class A3, class A4, class A5, class A6>
	void registerUniqueCallback( CallbackPriority priority,
		boost::_bi::bind_t< R, boost::_mfi::mf5<R, T, B1, B2, B3, B4, B5>,
			boost::_bi::list6<boost::_bi::value<A1*>, boost::_bi::value<A2>, boost::_bi::value<A3>, boost::_bi::value<A4>, boost::_bi::value<A5>, boost::_bi::value<A6> > > bind_object );

private:

	typedef std::deque< CallbackType > CallbackContainer;
	typedef std::map< CallbackPriority, CallbackContainer > PriorityCallbackType;
	typedef std::map< const CallbackType*, unsigned int > AccountingType;

private:

	template<typename R, typename F, typename L>
	bool hasFunction( CallbackPriority priority, const boost::_bi::bind_t<R, F, L>& external_func );

	void registerCallback( CallbackPriority priority, CallbackType callback );

	MergedCallbacks( const MergedCallbacks& );
	MergedCallbacks& operator=( const MergedCallbacks& );

private:

	AccountingType accounting_;
	PriorityCallbackType callbacks_;
};

template<typename R, typename F, typename L>
inline bool MergedCallbacks::hasFunction( CallbackPriority priority, const boost::_bi::bind_t<R, F, L>& external_func )
{
	typedef const boost::_bi::bind_t<R, F, L> basic_type;

	CallbackContainer& callbacks = callbacks_[ priority ];
	for( CallbackContainer::iterator itr = callbacks.begin(); itr != callbacks.end(); ++itr )
	{
		basic_type * internal_func = itr->target<basic_type>();
		if (internal_func == NULL)
		{
			continue;
		}

		if( external_func.compare( *internal_func ) )
		{
			accounting_[ &(*itr) ] += 1;
			return true;
		}
	}
	return false;
}

/* Static member or functions */

template<typename R>
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, R (*)(), boost::_bi::list0 > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

template<typename R, typename B1, typename A1 >
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, R(*)(B1), boost::_bi::list1<boost::_bi::value<A1> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

template<typename R, typename B1, typename B2, typename A1, typename A2 >
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, R(*)(B1, B2), boost::_bi::list2<boost::_bi::value<A1>, boost::_bi::value<A2> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

template<typename R, typename B1, typename B2, typename B3, typename A1, typename A2, typename A3 >
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, R(*)(B1, B2, B3), boost::_bi::list3<boost::_bi::value<A1>, boost::_bi::value<A2>, boost::_bi::value<A3> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

template<typename R, typename B1, typename B2, typename B3, typename B4, typename A1, typename A2, typename A3, typename A4 >
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, R(*)(B1, B2, B3, B4), boost::_bi::list4<boost::_bi::value<A1>, boost::_bi::value<A2>, boost::_bi::value<A3>, boost::_bi::value<A4> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

template<typename R, typename B1, typename B2, typename B3, typename B4, typename B5,  typename A1, typename A2, typename A3, typename A4, typename A5 >
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, R(*)(B1, B2, B3, B4, B5), boost::_bi::list5<boost::_bi::value<A1>, boost::_bi::value<A2>, boost::_bi::value<A3>, boost::_bi::value<A4>, boost::_bi::value<A5> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

/* Virtual or Member functions */

template<typename R, class T, class A1>
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, boost::_mfi::mf0<R, T>, boost::_bi::list1<boost::_bi::value<A1*> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

template<typename R, class T, class B1, class A1, class A2>
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, boost::_mfi::mf1<R, T, B1>, boost::_bi::list2<boost::_bi::value<A1*>, boost::_bi::value<A2> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

template<typename R, class T, class B1, class B2, class A1, class A2, class A3>
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, boost::_mfi::mf2<R, T, B1, B2>, boost::_bi::list3<boost::_bi::value<A1*>, boost::_bi::value<A2>, boost::_bi::value<A3> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

template<typename R, class T, class B1, class B2, class B3, class A1, class A2, class A3, class A4>
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, boost::_mfi::mf3<R, T, B1, B2, B3>, boost::_bi::list4<boost::_bi::value<A1*>, boost::_bi::value<A2>, boost::_bi::value<A3>, boost::_bi::value<A4> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

template<typename R, class T,
	class B1, class B2, class B3, class B4,
	class A1, class A2, class A3, class A4, class A5>
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, boost::_mfi::mf4<R, T, B1, B2, B3, B4>,
		boost::_bi::list5<boost::_bi::value<A1*>, boost::_bi::value<A2>, boost::_bi::value<A3>, boost::_bi::value<A4>, boost::_bi::value<A5> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}

template<typename R, class T,
	class B1, class B2, class B3, class B4, class B5,
	class A1, class A2, class A3, class A4, class A5, class A6>
inline void MergedCallbacks::registerUniqueCallback( CallbackPriority priority,
	boost::_bi::bind_t< R, boost::_mfi::mf5<R, T, B1, B2, B3, B4, B5>,
		boost::_bi::list6<boost::_bi::value<A1*>, boost::_bi::value<A2>, boost::_bi::value<A3>, boost::_bi::value<A4>, boost::_bi::value<A5>, boost::_bi::value<A6> > > bind_object )
{
	if( hasFunction( priority, bind_object ) )
		return;
	registerCallback( priority, bind_object );
}


#endif
