# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
    #Base image
    config.vm.box = "ubuntu/xenial64"

    #Host only network for sharing
    config.vm.network "private_network", ip: "192.168.222.222"

    #Shared folder
    config.vm.synced_folder ".", "/home/vagrant/Desktop/shared",
        SharedFoldersEnableSymlinksCreate: false
#        id: "core", :nfs => true, :nfs_version => 4, :mount_options => ['nolock,noatime']

    config.vm.provider :virtualbox do |vb|
        #Open virtualbox GUI
        vb.gui = true
        #Virtualbox CPU/Memory config
        vb.customize ["modifyvm", :id, "--memory", 1536]
        vb.customize ["modifyvm", :id, "--cpus", 1]
    end

    #Provision script for setup
    config.vm.provision "shell", inline: <<-SHELL
        echo "- Update packages list -"
        apt-get update
        echo "- Install xubuntu desktop enviroment -"
        apt-get install -y xubuntu-desktop
        echo "- Install NTP -"
        apt-get install -y ntp
        echo "- Install virtualbox guest addons -"
        apt-get install -y virtualbox-guest-dkms-hwe virtualbox-guest-utils-hwe
        echo "- Install compiling programs -"
        apt-get install -y build-essential cmake git
        echo "- Install libraries -"
        apt-get install -y libsdl2-dev
#libboost-dev 1.66 not available yet
        echo "- Setting lightdm -"
        echo "[SeatDefaults]" > /etc/lightdm/lightdm.conf.d/10-autologin.conf
        echo "autologin-user=vagrant" >> /etc/lightdm/lightdm.conf.d/10-autologin.conf
        systemctl start lightdm
        systemctl enable lightdm
        echo "- Finished provision script -"
    SHELL
end
