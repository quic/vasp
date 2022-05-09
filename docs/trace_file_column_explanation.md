# Trace file columns

The trace file generated after running a simulation is in the form of a Comma Separated Value (CSV) format. Each CSV contains the following columns:

|Column Header|Data Type|Description|
|-|-|-|
|`rv_id`|integer|identifier of the remote vehicle (transmitter of V2X message)|
|`hv_id`|integer|identifier of the host vehicle (receiver of V2X message)|
|`target_id`|integer|identifier of the target vehicle of the attack. This is especially useful for ghost-node based attacks to understand which vehicle is the target.|
|`msg_generation_time`|double|timestamp of when the message was generated at the transmitter|
|`msg_rcv_time`|double|timestamp of when the message was received at the receiver|
|`rv_msg_count`|integer|message sequence identifier or message count of the message transmitted by remote vehicle. This goes from 0 to 127 and resets to 0.|
|`rv_wsm_data`|string|additional data about the message|
|`rv_pos_x`|double|latitude of transmitter|
|`rv_pos_y`|double|longitude of transmitter|
|`rv_pos_z`|double|elevation of transmitter|
|`rv_speed`|double|speed of transmitter|
|`rv_accel`|double|acceleration of transmitter|
|`rv_heading`|double|heading/bearing/direction of transmitter|
|`rv_yaw_rate`|double|yaw-rate of transmitter|
|`rv_length`|double|length of transmitter vehicle|
|`rv_width`|double|width of transmitter vehicle|
|`rv_height`|double|height of transmitter vehicle|
|`hv_msg_count`|integer|message sequence identifier or message count of the message about to be transmitted by host vehicle. This goes from 0 to 127 and resets to 0.|
|`hv_wsm_data`|string|additional data about the message|
|`hv_pos_x`|double|latitude of receiver|
|`hv_pos_y`|double|longitude of receiver|
|`hv_pos_z`|double|elevation of receiver|
|`hv_speed`|double|speed of receiver|
|`hv_accel`|double|acceleration of receiver|
|`hv_heading`|double|heading/bearing/direction of receiver|
|`hv_length`|double|length of receiving vehicle|
|`hv_width`|double|width of receiving vehicle|
|`hv_height`|double|height of receiving vehicle|
|`attack_type`|string|type of attack if malicious/attacker vehicle, otherwise defaults to "Genuine"|
|`eebl_warn`|boolean|indicates if EEBL raised a warning; 1 = warning; 0 = no warning|
|`ima_warn`|boolean|indicates if IMA raised a warning; 1 = warning; 0 = no warning|