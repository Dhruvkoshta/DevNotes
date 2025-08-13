___
## ***Used in ChatPulse***

Here is your code converted to Markdown with syntax highlighting for JavaScript/TypeScript:

```typescript
import { headers } from 'next/headers';
import { NextResponse } from 'next/server';
import type { NextRequest } from 'next/server';
import { auth } from './lib/auth';

const publicPaths = [
  '/',
  '/signin',
  '/api/auth',
  '/info',
  '/banner.png',
];

export async function middleware(request: NextRequest) {
  const { pathname } = request.nextUrl;

  const isPublicPath = publicPaths.some(
    (path) =>
      pathname === path ||
      pathname.startsWith(`${path}/`) ||
      pathname.startsWith('/api/auth/')
  );

  const isAuthRoute = pathname.startsWith('/signin');

  const session = await auth.api.getSession({
    headers: await headers(),
  });

  if (isAuthRoute && session) {
    return NextResponse.redirect(new URL('/home', request.url));
  }

  if (!isPublicPath && !session) {
    const signInUrl = new URL('/signin', request.url);
    signInUrl.searchParams.set('callbackUrl', request.url);
    return NextResponse.redirect(signInUrl);
  }

  return NextResponse.next();
}

export const config = {
  matcher: [
    '/((?!_next/static|_next/image|favicon\\.ico|public).*)',
  ],
};
```


## ***Simple middleware***

```typescript
import { NextRequest, NextResponse } from "next/server";
import { headers } from "next/headers";
import { auth } from "@/lib/auth";

export async function middleware(request: NextRequest) {
  const session = await auth.api.getSession({
    headers: await headers(),
  });

  if (!session) {
    return NextResponse.redirect(new URL("/sign-in", request.url));
  }

  return NextResponse.next();
}

export const config = {
  matcher: ["/((?!api|_next/static|_next/image|favicon.ico|sign-in|assets).*)"],
};

```

___



