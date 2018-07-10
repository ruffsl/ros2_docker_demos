# SROS2 Demo

To demonstrate SROS2 using Docker, we extend from the classic talker listener example into separate containers while using volumes to mount the cryptographic artifacts or keystore necessary for each SROS2 node. To simplify setup, we'll use `ros2:bouncy-basic` as the base image for additional supported rwm implementations.

> Begin by opening a shell and changing the current direct to this repo folder:

```
cd <path-to>/ros2_docker_demos/sros2
```

> Should you want to use an rwm relying on RTI Connext for security, you will need to provide a license file at runtime. This can be done here by copying your license file into the demo sub-directory used when creating the docker volume. You may gain a license here:
https://www.rti.com/downloads

```
cp <path-to>/rti_license.dat `pwd`/sros2_demo
```

> Now we can create the volume for the keystore and bootstrap it with the necessary files

```
docker volume create sros2_demo

docker run --rm \
  --volume `pwd`/sros2_demo:/tmp \
  --volume sros2_demo:/sros2_demo \
  busybox \
  cp -TRv /tmp /sros2_demo
```

> Next we must generate the keystore by with the required cryptographic artifacts using the setup scrip we copied into the volume prior.

```
docker run -it --rm \
  --volume sros2_demo:/sros2_demo \
  --workdir /sros2_demo \
  osrf/ros2:bouncy-basic \
  ./keystore_setup.bash
```

> Finally we can use docker-compose to build and launch our secure talker listener example. You may wish to read through the compose file while the image is being built to observe the necessary environment variables for runtime deployment.

```
docker-compose up
```
