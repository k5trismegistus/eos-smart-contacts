import { Api, Rpc, SignatureProvider } from 'eosjs';
import { RPC_URL, CONTRACT_ACCOUNT, USER_ACCOUNT, USER_PRIVATE_KEY } from './constants';

export const startGame = async () => {
  await takeAction('start', { name: USER_ACCOUNT })
  return await getGame()
}

export const getGame = async () => {
  try {
    const rpc = new Rpc.JsonRpc(RPC_URL);
    const result = await rpc.get_table_rows({
      "json": true,
      "code": CONTRACT_ACCOUNT,    // contract who owns the table
      "scope": CONTRACT_ACCOUNT,   // scope of the table
      "table": "game",    // name of the table as specified by the contract abi
      "limit": 1,
      "upper_bound": USER_ACCOUNT,
    });
    console.log(result.rows)
    return result.rows[0];
  } catch (err) {
    console.error(err);
  }
}

const takeAction = async (action, dataValue) => {
  const privateKey = USER_PRIVATE_KEY;
  const rpc = new Rpc.JsonRpc(RPC_URL);
  const signatureProvider = new SignatureProvider([privateKey]);
  const api = new Api({ rpc, signatureProvider, textDecoder: new TextDecoder(), textEncoder: new TextEncoder() });

  // Main call to blockchain after setting action, account_name and data
  try {
    const resultWithConfig = await api.transact({
      actions: [{
        account: CONTRACT_ACCOUNT,
        name: action,
        authorization: [{
          actor: USER_ACCOUNT,
          permission: 'active',
        }],
        data: dataValue,
      }]
    }, {
      blocksBehind: 3,
      expireSeconds: 30,
    });
    return resultWithConfig;
  } catch (err) {
    console.error(err.json)
    throw(err)
  }
}