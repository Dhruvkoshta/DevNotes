## Error Handling

Handling Expected Errors from Server Actions

> const [state, FormAction, pending] = useActionState( createUser, initialState )

Use the useActionState hook to manage the state of Server Actions, including handling errors. This approach avoids try/catch blocks for expected errors,

---

## Loading UI and Streaming

![](https://beta.appflowy.cloud/api/file_storage/5280cd98-0615-4f27-b162-2f7e8f5d0dbe/v1/blob/4a0e06d7%2D04e2%2D5c99%2Dad75%2D23e1bca86797/BFFaB5ByU4F_iBPosiusqdFuvURpyyVtgLUCjzb3GzM=.png)  

## Streaming without Suspense :

![](https://beta.appflowy.cloud/api/file_storage/5280cd98-0615-4f27-b162-2f7e8f5d0dbe/v1/blob/4a0e06d7%2D04e2%2D5c99%2Dad75%2D23e1bca86797/A_pHbXVkPtxSnZP7hHGt5B29KtuQXnszy7a83yibT5Y=.png)

## Streaming with Suspense :

![](https://beta.appflowy.cloud/api/file_storage/5280cd98-0615-4f27-b162-2f7e8f5d0dbe/v1/blob/4a0e06d7%2D04e2%2D5c99%2Dad75%2D23e1bca86797/DebToGILPUo3iTRI7BbPfDmGHY-k9MSnIeShyb4UQt4=.png)
## Streaming with Suspense :

![](https://beta.appflowy.cloud/api/file_storage/5280cd98-0615-4f27-b162-2f7e8f5d0dbe/v1/blob/4a0e06d7%2D04e2%2D5c99%2Dad75%2D23e1bca86797/DebToGILPUo3iTRI7BbPfDmGHY-k9MSnIeShyb4UQt4=.png)
## Server Components:

we can't use state, because state can change, but Server Components can't re-render. And we can't use effects because effects only run _after_ the render, on the client, and Server Components never make it to the client.

It also means that we have a bit more flexibility when it comes to the rules. For example, in traditional React, we need to put side effects inside a `useEffect` callback or an event handler or something, so that they don't repeat on every render. But if the component only runs _once_, we don't have to worry about that!



