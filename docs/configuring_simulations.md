# Configuring your simulation

The `omnetpp.ini` file provides a couple of configuration options for running your simulations.
|Option|Description|
|-|-|
|`attackPolicy`|controls which attack policy to use to perform the attacks. |
|`sporadicInsertionRate`|controls the rate of attack insertion when sporadic attack policy is chosen through `attackPolicy` option.|
|`maliciousProbability`|option controls the distribution of genuine vs attacker vehicles inserted into the simulation. E.g., `maliciousProbability` of `0.3` means, off all the vehicles in the simulation, 30% will be attackers|
|`attackType`|option controls the attack to perform in the simulation. Please refer to the `<path/to/veins>/src/vasp/attack/Type.h` file to find out the number-to-attack mapping.|
|`nDosMessages`|controls the number of messages to be transmitted for each Denial of Service attack|
|`posAttackOffset`|This option is used by position offset type attacks (random and constant) to control the offset from real position.|
|`dimensionAttackOffset`|This option is used by dimension/length/width offset type attacks (random and constant) to control the offset from real position.|
|`headingAttackOffset`|This option is used by heading offset type attacks (random and constant) to control the offset from real position.|
|`yawRateAttackOffset`|This option is used by yaw-rate offset type attacks (random and constant) to control the offset from real position.|
|`accelerationAttackOffset`|This option is used by acceleration offset type attacks (random and constant) to control the offset from real position.|
|`speedAttackOffset`|This option is used by speed offset type attacks (random and constant) to control the offset from real position.|