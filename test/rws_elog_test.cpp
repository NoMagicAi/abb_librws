#include <gtest/gtest.h>

#include <abb_librws/v2_0/rw/elog.h>

#include <stdexcept>
#include <string>

namespace abb ::rws ::v2_0 ::rw ::elog
{
    namespace
    {
        // The reply of GET /ctrl/registry/elogtext on an OmniCore with RobotWare 7.21.0, cut to two entries.
        std::string const REGISTRY_REPLY =
            R"(<?xml version="1.0" encoding="utf-8"?><html xmlns="http://www.w3.org/1999/xhtml"> <head> )"
            R"(<base href="https://10.2.0.117:443/ctrl/registry/elogtext/" /> </head> <body>  <div class="state">  )"
            R"(<a href="" rel="self"></a> <ul> <li class="ctrl-reg-file-li" title="elogtext"> <span class="elogtext">)"
            R"(<![CDATA[<?xml version = "1.0" encoding="utf-8" ?>
<elogtext_registry xmlns="urn:abb-robotics-registry-elog-text">
<entry domain_no="5" min="0" max="9999" prepath="$PRODUCTS/RobotControl_7.21.0/language/install/../" postpath="/controller/elog/moc_elogtext.xml"/>
<domains prepath="$PRODUCTS/RobotControl_7.21.0/language/install/../" postpath="/controller/elog/elog_domains.xml"/>
</elogtext_registry>]]>)"
            R"(</span> </li> </ul></div> </body></html>)";
    }

    TEST(ElogTextRegistryTest, ReturnsTheRegistryDocumentFromTheCdataSection)
    {
        std::string const registry = parseElogTextRegistry(REGISTRY_REPLY);

        EXPECT_EQ(registry.find("<?xml version = \"1.0\""), 0u);
        EXPECT_NE(registry.find("<elogtext_registry xmlns=\"urn:abb-robotics-registry-elog-text\">"), std::string::npos);
        EXPECT_NE(registry.find("postpath=\"/controller/elog/moc_elogtext.xml\""), std::string::npos);
        EXPECT_NE(registry.find("</elogtext_registry>"), std::string::npos);
    }

    TEST(ElogTextRegistryTest, ThrowsWhenTheReplyHasNoRegistry)
    {
        std::string const reply =
            R"(<?xml version="1.0" encoding="utf-8"?><html xmlns="http://www.w3.org/1999/xhtml"><body>)"
            R"(<div class="state"><ul></ul></div></body></html>)";

        EXPECT_THROW(parseElogTextRegistry(reply), std::invalid_argument);
    }
}
