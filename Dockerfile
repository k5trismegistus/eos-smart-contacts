FROM eosio/eos-dev:v1.2.4
RUN echo 'alias cleos="cleos -u http://nodeos:8888"' >> ~/.bashrc
ADD ./scripts/ /scripts/
RUN chmod 777 /scripts/setup.sh
ENTRYPOINT ["/scripts/setup.sh"]