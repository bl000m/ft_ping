## Statistics

# stddev (standard deviation)
calculate  the square root of the average of the squared differences between each individual RTT and the mean RTT

- Exemple:
Given the RTT values:
RTT1 = 12.906 ms
RTT2 = 14.064 ms
RTT3 = 14.114 ms
RTT4 = 14.087 ms

1. Calculate the mean:
Mean = (12.906+14.064+14.114+14.0874) / 4 => 55.171 / 4 => 13.79275 ms

2. Calculate the sum of squared differences for each RTT:

(RTT1 - Mean)^2 = (12.906 - 13.79275)^2 = (-0.88675)^2 = 0.78503 ms^2
(RTT2 - Mean)^2 = (14.064 - 13.79275)^2 = (0.27125)^2 = 0.07353 ms^2
(RTT3 - Mean)^2 = (14.114 - 13.79275)^2 = (0.32125)^2 = 0.10332 ms^2
(RTT4 - Mean)^2 = (14.087 - 13.79275)^2 = (0.29425)^2 = 0.08648 ms^2

3. Sum up the squared differences:
0.78503 + 0.07353 + 0.10332 + 0.08648 = 1.04836 ms^2

4. Divide the sum of squared differences by the number of packets received:
Standard Deviation = 1.04836 / 4 = 0.26209 ≈ 0.512 ms

So, the calculated standard deviation for the given RTT values is approximately 0.512 ms.

