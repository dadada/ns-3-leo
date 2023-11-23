# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('leo', ['core','internet', 'propagation', 'stats', 'traffic', 'flow-monitor', 'applications'])
    module.source = [
        'model/isl-mock-channel.cc',
        'model/isl-propagation-loss-model.cc',
        'helper/isl-helper.cc',
        'model/leo.cc',
        'helper/leo-channel-helper.cc',
        'model/leo-mock-channel.cc',
        'model/leo-mock-net-device.cc',
        'helper/leo-helper.cc',
        'model/leo-propagation-loss-model.cc',
        'model/mock-net-device.cc',
        'model/mock-channel.cc',
        'helper/nd-cache-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('leo')
    module_test.source = [
        'test/leo-test-suite.cc',
        'test/isl-test-suite.cc',
        'test/isl-mock-channel-test-suite.cc',
        'test/leo-mock-channel-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'leo'
    headers.source = [
        'model/isl-mock-channel.h',
        'model/isl-propagation-loss-model.h',
        'helper/isl-helper.h',
        'model/leo.h',
        'helper/leo-channel-helper.h',
        'model/leo-mock-channel.h',
        'model/leo-mock-net-device.h',
        'helper/leo-helper.h',
        'model/leo-propagation-loss-model.h',
        'model/mock-net-device.h',
        'model/mock-channel.h',
        'helper/nd-cache-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

