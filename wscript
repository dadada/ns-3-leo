# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('leo', ['core','internet', 'propagation', 'stats', 'flow-monitor', 'applications', 'netanim'])
    module.source = [
        'helper/arp-cache-helper.cc',
        'helper/isl-helper.cc',
        'helper/leo-channel-helper.cc',
        'helper/leo-input-fstream-container.cc',
        'helper/leo-orbit-node-helper.cc',
        'helper/nd-cache-helper.cc',
        'helper/ground-node-helper.cc',
        'helper/satellite-node-helper.cc',
        'model/leo-circular-orbit-mobility-model.cc',
        'model/leo-circular-orbit-position-allocator.cc',
        'model/leo-mock-channel.cc',
        'model/leo-mock-net-device.cc',
        'model/leo-orbit.cc',
        'model/leo-lat-long.cc',
        'model/leo-polar-position-allocator.cc',
        'model/leo-propagation-loss-model.cc',
        'model/mock-net-device.cc',
        'model/mock-channel.cc',
        'model/isl-mock-channel.cc',
        'model/isl-propagation-loss-model.cc',
        ]

    module_test = bld.create_ns3_module_test_library('leo')
    module_test.source = [
        'test/ground-node-helper-test-suite.cc',
        'test/isl-mock-channel-test-suite.cc',
        'test/isl-propagation-test-suite.cc',
        'test/isl-test-suite.cc',
        'test/leo-anim-test-suite.cc',
        'test/leo-orbit-test-suite.cc',
        'test/leo-input-fstream-container-test-suite.cc',
        'test/leo-mobility-test-suite.cc',
        'test/leo-mock-channel-test-suite.cc',
        'test/leo-propagation-test-suite.cc',
        'test/leo-test-suite.cc',
        'test/leo-trace-test-suite.cc',
        'test/satellite-node-helper-test-suite.cc',
    ]

    headers = bld(features='ns3header')
    headers.module = 'leo'
    headers.source = [
        'helper/arp-cache-helper.h',
        'helper/isl-helper.h',
        'helper/leo-channel-helper.h',
        'helper/leo-input-fstream-container.h',
        'helper/leo-orbit-node-helper.h',
        'helper/nd-cache-helper.h',
        'helper/ground-node-helper.h',
        'helper/satellite-node-helper.h',
        'model/leo-circular-orbit-mobility-model.h',
        'model/leo-circular-orbit-position-allocator.h',
        'model/leo-mock-channel.h',
        'model/leo-mock-net-device.h',
        'model/leo-oneweb-constants.h',
        'model/leo-orbit.h',
        'model/leo-lat-long.h',
        'model/leo-polar-position-allocator.h',
        'model/leo-propagation-loss-model.h',
	'model/leo-starlink-constants.h',
	'model/leo-telesat-constants.h',
        'model/mock-net-device.h',
        'model/mock-channel.h',
        'model/isl-mock-channel.h',
        'model/isl-propagation-loss-model.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

