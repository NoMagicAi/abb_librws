/***********************************************************************************************************************
 *
 * Copyright (c) 2022, NoMagic sp. z o.o.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that
 * the following conditions are met:
 *
 *    * Redistributions of source code must retain the
 *      above copyright notice, this list of conditions
 *      and the following disclaimer.
 *    * Redistributions in binary form must reproduce the
 *      above copyright notice, this list of conditions
 *      and the following disclaimer in the documentation
 *      and/or other materials provided with the
 *      distribution.
 *    * Neither the name of ABB nor the names of its
 *      contributors may be used to endorse or promote
 *      products derived from this software without
 *      specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***********************************************************************************************************************
 */
#include <abb_librws/rws_resource.h>

#include <iostream>
#include <boost/bimap.hpp>
#include <boost/assign.hpp>


namespace abb :: rws
{

std::ostream& operator<<(std::ostream& os, RAPIDResource const& resource)
{
    return os
        << "RAPIDResource("
        << ".task=" << resource.task
        << ", .module=" << resource.module
        << ", .name=" << resource.name
        << ")";
}

std::ostream& operator<<(std::ostream& os, CFGDomain const& domain)
{
    return os << to_string(domain);
}

boost::bimap<CFGDomain, std::string> makeCFGDomainMap()
{
    boost::bimap<CFGDomain, std::string> map;
    map.insert(boost::bimap<CFGDomain, std::string>::relation(CFGDomain::D_EIO, "EIO"));
    map.insert(boost::bimap<CFGDomain, std::string>::relation(CFGDomain::D_MMC, "MMC"));
    map.insert(boost::bimap<CFGDomain, std::string>::relation(CFGDomain::D_MOC, "MOC"));
    map.insert(boost::bimap<CFGDomain, std::string>::relation(CFGDomain::D_PROC, "PROC"));
    map.insert(boost::bimap<CFGDomain, std::string>::relation(CFGDomain::D_SIO, "SIO"));
    map.insert(boost::bimap<CFGDomain, std::string>::relation(CFGDomain::D_SYS, "SYS"));
    return map;
}

static boost::bimap<CFGDomain, std::string> const CFGDomainMap = makeCFGDomainMap();

CFGDomain to_CFGDomain(std::string const& domain)
{
    try{
        return CFGDomainMap.right.at(domain);
    } catch (std::out_of_range const& e) {
        BOOST_THROW_EXCEPTION(std::runtime_error("Unknown CFGDomain: " + domain));
    }
}

std::string to_string(CFGDomain domain)
{
    try {
        return CFGDomainMap.left.at(domain);
    } catch (std::out_of_range const& e) {
        BOOST_THROW_EXCEPTION(std::runtime_error("Unknown CFGDomain: " + std::to_string(static_cast<int>(domain))));
    }
}

} // end namespace abb::rws
