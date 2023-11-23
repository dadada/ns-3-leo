
    .AddAttribute ("MobilityModel", "The mobility model of the device",
                   PointerValue (),
                   MakePointerAccessor (&MockNetDevice::SetMobilityModel,
                     &MockNetDevice::GetMobilityModel),
                   MakePointerChecker<MobilityModel> ())
