[General]
network = ${targetTypeName}
#record-eventlog = true
#eventlog-message-detail-pattern = *:(not declaredOn(cMessage) and not declaredOn(cNamedObject) and not declaredOn(cObject))

num-rngs = 3
**.mobility.rng-0 = 1
**.wlan.mac.rng-0 = 2
#debug-on-errors = true

tkenv-plugin-path = ../../../etc/plugins

*.playgroundSizeX = 600
*.playgroundSizeY = 400
**.debug = true
**.coreDebug = false
**.host*.**.channelNumber = 0

# channel physical parameters
*.channelcontrol.carrierFrequency = 2.4GHz
*.channelcontrol.pMax = 2.0mW
*.channelcontrol.sat = -110dBm
*.channelcontrol.alpha = 2
*.channelcontrol.numChannels = 1

# mobility
**.host*.mobility.x = -1
**.host*.mobility.y = -1

**.host*.mobilityType = "inet.mobility.MassMobility"
**.host*.mobility.changeInterval = truncnormal(2s, 0.5s)
**.host*.mobility.changeAngleBy = normal(0deg, 30deg)
**.host*.mobility.speed = truncnormal(20mps, 8mps)
**.host*.mobility.updateInterval = 100ms

# ping app (host[0] pinged by others)
*.host[0].pingApp.destAddr = ""
*.host[*].pingApp.destAddr = "host[0]"
**.pingApp.startTime = uniform(1s,5s)

# nic settings
**.wlan.mgmt.frameCapacity = 10
**.wlan.mac.address = "auto"
**.wlan.mac.maxQueueSize = 14
**.wlan.mac.rtsThresholdBytes = 3000B
**.wlan.mac.bitrate = 2Mbps
**.wlan.mac.retryLimit = 7
**.wlan.mac.cwMinData = 7
**.wlan.mac.cwMinBroadcast = 31

**.wlan.radio.bitrate = 2Mbps
**.wlan.radio.transmitterPower = 2mW
**.wlan.radio.thermalNoise = -110dBm
**.wlan.radio.sensitivity = -85mW
**.wlan.radio.pathLossAlpha = 2
**.wlan.radio.snirThreshold = 4dB


*.numHosts = ${numOfHosts}
