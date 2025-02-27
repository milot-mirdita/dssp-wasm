// Copyright Maarten L. Hekkelman, Radboud University 2008-2011.
// Copyright Coos Baakman, Jon Black, Wouter G. Touw & Gert Vriend, Radboud university medical center 2015.
//   Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE_1_0.txt or copy at
//             http://www.boost.org/LICENSE_1_0.txt)
//
// buffer is a thread safe queue

#ifndef DSSP_BUFFER_H
#define DSSP_BUFFER_H

#pragma once

#include "mas.h"

#ifdef HAVE_BOOST_THREAD
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#endif

#include <deque>

template<class T, uint32 N = 100>
class buffer
{
  public:

            buffer() {}

  void        put(T inValue);
  T          get();

  private:
            buffer(const buffer&);
  buffer&        operator=(const buffer&);

  std::deque<T>    m_queue;
#ifdef HAVE_BOOST_THREAD
  boost::mutex    m_mutex;
  boost::condition  m_empty, m_full;
#endif
};

template<class T, uint32 N>
void buffer<T,N>::put(T inValue)
{
#ifdef HAVE_BOOST_THREAD
  boost::mutex::scoped_lock lock(m_mutex);

  while (m_queue.size() >= N)
    m_full.wait(lock);
#endif

  m_queue.push_back(inValue);

#ifdef HAVE_BOOST_THREAD
  m_empty.notify_one();
#endif
}

template<class T, uint32 N>
T buffer<T,N>::get()
{
#ifdef HAVE_BOOST_THREAD
  boost::mutex::scoped_lock lock(m_mutex);

  while (m_queue.empty())
    m_empty.wait(lock);
#endif

  T result = m_queue.front();
  m_queue.pop_front();

#ifdef HAVE_BOOST_THREAD
  m_full.notify_one();
#endif

  return result;
}

#endif
