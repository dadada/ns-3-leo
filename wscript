# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('leo', ['core','internet', 'propagation', 'stats', 'traffic', 'flow-monitor', 'applications'])
    module.source = [
        'helper/arp-cache-helper.cc',
        'helper/isl-helper.cc',
        'helper/leo-channel-helper.cc',
        'helper/nd-cache-helper.cc',
        'helper/ground-node-helper.cc',
        'helper/satellite-node-helper.cc',
        'model/leo.cc',
        'model/leo-mock-channel.cc',
        'model/leo-mock-net-device.cc',
        'model/leo-propagation-loss-model.cc',
        'model/mock-net-device.cc',
        'model/mock-channel.cc',
        'model/isl-mock-channel.cc',
        'model/isl-propagation-loss-model.cc',
        'utils/leo-input-fstream-container.cc',
        ]

    module_test = bld.create_ns3_module_test_library('leo')
    module_test.source = [
        'test/ground-node-helper-test-suite.cc',
        'test/isl-mock-channel-test-suite.cc',
        'test/isl-propagation-test-suite.cc',
        'test/isl-test-suite.cc',
        'test/leo-input-fstream-container-test-suite.cc',
        'test/leo-mobility-test-suite.cc',
        'test/leo-mock-channel-test-suite.cc',
        'test/leo-propagation-test-suite.cc',
        'test/leo-test-suite.cc',
        'test/satellite-node-helper-test-suite.cc',
    ]

    headers = bld(features='ns3header')
    headers.module = 'leo'
    headers.source = [
        'helper/arp-cache-helper.h',
        'helper/isl-helper.h',
        'helper/leo-channel-helper.h',
        'helper/nd-cache-helper.h',
        'helper/ground-node-helper.h',
        'helper/satellite-node-helper.h',
        'model/leo.h',
        'model/leo-mock-channel.h',
        'model/leo-mock-net-device.h',
        'model/leo-propagation-loss-model.h',
        'model/mock-net-device.h',
        'model/mock-channel.h',
        'model/isl-mock-channel.h',
        'model/isl-propagation-loss-model.h',
        'utils/leo-input-fstream-container.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

