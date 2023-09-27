# firewall-hub

This is the business logic for the central software hub. This logic should be monitoring network activity of the underlying OS (OpenBSD) through kernel hooks and send out notifications to alerting devices. This logic will also keep track of persistent state regarding firewall rules and synchronize with other firewall packages such as Little Snitch.
